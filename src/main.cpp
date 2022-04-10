#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <memory>
#include <ratio>
#include <string>
#include <thread>
#include <vector>

#include "../lib/config_model.h"
#include "../lib/controller_model.h"
#include "../lib/maze_model.h"

using AtomicControllerModel = std::atomic<ControllerModel>;
using AtomicConfigModel = std::atomic<ConfigModel>;
using AtomicMazeModel = std::atomic<MazeModel>;

struct DrawArea
{
  int x{};
  int y{};
};

constexpr int CONTROL_AREA_WIDTH = 100;
constexpr int CONTROL_AREA_HEIGHT = 100;
constexpr Position<> CONTROL_AREA_CENTER = { CONTROL_AREA_WIDTH / 2, CONTROL_AREA_HEIGHT / 2 };
constexpr float CONTROL_CIRCLE_FACTOR = 0.7F;
constexpr DrawArea maze_area{ 0, 0 };
constexpr DrawArea control_area{ CONTROL_AREA_WIDTH, 0 };

bool is_left_button_pressed(const ftxui::Mouse &mouse)
{
  auto left_button = mouse.button == ftxui::Mouse::Button::Left;
  auto pressed = mouse.motion == ftxui::Mouse::Motion::Pressed;
  return pressed && left_button;
}


void draw_maze(ftxui::Canvas &canvas, DrawArea draw_area, const MazeModel &maze_model)
{
  constexpr static int BALL_RADIUS = 3;

  const auto &ball_pos = maze_model.ball.position.rounded();
  auto x = ball_pos.x + draw_area.x;
  auto y = ball_pos.y + draw_area.y;
  canvas.DrawPointCircleFilled(x, y, BALL_RADIUS);
}

void draw_control_area(ftxui::Canvas &canvas, DrawArea draw_area, const AtomicControllerModel &controller_model)
{
  constexpr static int max_radius = static_cast<int>(
    static_cast<float>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5F * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;
  auto pos = controller_model.load().limited_block_position();

  auto x_off = [&draw_area](int x) { return draw_area.x + x; };
  auto y_off = [&draw_area](int y) { return draw_area.y + y; };

  canvas.DrawPointCircle(x_off(CONTROL_AREA_CENTER.x), y_off(CONTROL_AREA_CENTER.y), max_radius);
  canvas.DrawBlockCircleFilled(x_off(pos.x), y_off(pos.y), indicator_radius);
}

void draw_debug(ftxui::Canvas &canvas,
  const AtomicControllerModel &a_controller_model,
  const AtomicMazeModel &a_maze_model)
{
  const auto &controller_model = a_controller_model.load();
  const auto &maze_model = a_maze_model.load();

  static auto now = std::chrono::steady_clock::now();
  auto frame_time_ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - now).count();
  auto frame_time_s = static_cast<float>(frame_time_ms) / 1'000.F;// NOLINT
  auto fps = 1.F / frame_time_s;

  [[maybe_unused]] const auto pos = controller_model.limited_block_position();
  [[maybe_unused]] const auto mouse_pos = controller_model.mouse_position;
  std::array debug_lines{
    fmt::format("ctrl x: {}", pos.x),
    fmt::format("ctrl y: {}", pos.y),
    fmt::format("mouse x: {}", mouse_pos.x),
    fmt::format("mouse y: {}", mouse_pos.y),
    fmt::format("ball.v.x: {}", maze_model.ball.velocity.x),
    fmt::format("ball.v.y: {}", maze_model.ball.velocity.y),
    fmt::format("maze.a.x: {}", maze_model.acceleration.x),
    fmt::format("maze.a.y: {}", maze_model.acceleration.y),
    fmt::format("frame time: {}", frame_time_s),
    fmt::format("fps: {}", fps),
  };

  int counter = 0;

  for (const auto &line : debug_lines) { canvas.DrawText(0, (counter++) * 4, line); }

  now = std::chrono::steady_clock::now();
}

bool handle_key_event(const ftxui::Event &e,
  AtomicConfigModel &a_config_model,
  const ftxui::ScreenInteractive::Callback &exit)
{
  if (e == ftxui::Event::Character('d')) {
    auto config_model = a_config_model.load();

    config_model.debug = !config_model.debug;
    a_config_model.store(config_model);

    return true;
  } else if (e == ftxui::Event::Character('q')) {
    exit();

    return true;
  }
  return false;
}

bool handle_button_event(ftxui::Event &e,

  AtomicMazeModel &a_maze_model,
  AtomicControllerModel &a_controller_model)
{
  const auto &mouse = e.mouse();
  auto controller_model = a_controller_model.load();

  if (is_left_button_pressed(mouse)) {
    auto maze_model = a_maze_model.load();

    controller_model.mouse_position = { mouse.x * 2 - control_area.x, mouse.y * 4 - control_area.y };
    maze_model.acceleration = controller_model.limited_direction() * (1.0F / 100'000.0F);// NOLINT
    // maze_model.tick();

    a_controller_model.store(controller_model);
    a_maze_model.store(maze_model);

    return true;
  }

  if (mouse.button == ftxui::Mouse::Button::Left && mouse.motion == ftxui::Mouse::Motion::Released) {
    controller_model.mouse_position = CONTROL_AREA_CENTER;
    a_controller_model.store(controller_model);
    return true;
  }

  return false;
}


ftxui::Component add_events(ftxui::Component component,
  AtomicConfigModel &a_config_model,
  AtomicMazeModel &a_maze_model,
  AtomicControllerModel &a_controller_model,
  const ftxui::ScreenInteractive::Callback &exit)
{
  return ftxui::CatchEvent(std::move(component), [&](ftxui::Event e) {
    if (e.is_character()) {
      return handle_key_event(e, a_config_model, exit);
    } else if (e.is_mouse()) {
      return handle_button_event(e, a_maze_model, a_controller_model);
    }

    return false;
  });
}

int main()
{
  std::atomic<ControllerModel> controller_model;
  controller_model.store({
    CONTROL_AREA_CENTER,
    CONTROL_AREA_CENTER,
    static_cast<float>(CONTROL_AREA_WIDTH) * 0.5F * CONTROL_CIRCLE_FACTOR// NOLINT Magic Number
  });

  std::atomic<ConfigModel> config_model{};
  std::atomic<MazeModel> maze_model{};


  auto maze_control_renderer = ftxui::Renderer([&] {
    ftxui::Canvas canvas{ CONTROL_AREA_WIDTH * 2, CONTROL_AREA_HEIGHT };
    draw_maze(canvas, maze_area, maze_model);
    draw_control_area(canvas, control_area, controller_model);
    if (config_model.load().debug) { draw_debug(canvas, controller_model, maze_model); }

    return ftxui::canvas(std::move(canvas));
  });

  std::atomic_bool stop{ false };

  auto ticker = std::thread([&maze_model, &stop] {
    auto now = std::chrono::steady_clock::now;
    auto last = now();

    while (!stop) {
      std::this_thread::sleep_until(last + std::chrono::milliseconds(1));

      auto mm = maze_model.load();
      mm.tick();
      // mm.acceleration = { 0.0F, 0.0F };
      maze_model.store(mm);

      last = now();
    }
  });


  auto screen = ftxui::ScreenInteractive::FitComponent();

  auto screen_refresh = std::thread([&screen, &stop] {
    auto now = std::chrono::steady_clock::now;
    auto last = now();
    constexpr static std::size_t REFRESH_RATE = 60;
    using fps = std::ratio<1, REFRESH_RATE>;
    constexpr static std::chrono::duration single_frame = std::chrono::duration<std::size_t, fps>(1);

    while (!stop) {
      std::this_thread::sleep_until(last + single_frame);
      screen.PostEvent(ftxui::Event::Custom);

      last = now();
    }
  });

  screen.Loop(add_events(maze_control_renderer, config_model, maze_model, controller_model, screen.ExitLoopClosure()));
  stop = true;
  ticker.join();
  screen_refresh.join();
}
