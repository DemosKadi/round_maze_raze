#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "../lib/config_model.h"
#include "../lib/controller_model.h"
#include "../lib/maze_model.h"


constexpr int CONTROL_AREA_WIDTH = 100;
constexpr int CONTROL_AREA_HEIGHT = 100;
constexpr Position<> CONTROL_AREA_CENTER = { CONTROL_AREA_WIDTH / 2, CONTROL_AREA_HEIGHT / 2 };
constexpr float CONTROL_CIRCLE_FACTOR = 0.7F;

bool is_left_button_pressed(const ftxui::Mouse &mouse)
{
  auto left_button = mouse.button == ftxui::Mouse::Button::Left;
  auto pressed = mouse.motion == ftxui::Mouse::Motion::Pressed;
  return pressed && left_button;
}

// FIXME: use reference instead of shared_ptr

ftxui::Component show_maze(ControllerModel &controller_model, MazeModel &maze_model)
{
  constexpr static int WIDTH = CONTROL_AREA_WIDTH;
  constexpr static int HEIGHT = CONTROL_AREA_HEIGHT;
  constexpr static int BALL_RADIUS = 3;
  // constexpr static std::size_t TICKS_PER_SECOND = 100;

  auto maze_drawer = [&controller_model, &maze_model] {
    maze_model.acceleration = controller_model.limited_direction();
    maze_model.tick();

    auto c = ftxui::Canvas(WIDTH, HEIGHT);
    const auto &ball_pos = maze_model.ball.position.rounded();
    c.DrawPointCircleFilled(ball_pos.x, ball_pos.y, BALL_RADIUS);

    return ftxui::canvas(std::move(c));
  };

  return ftxui::Renderer(maze_drawer);
}

ftxui::Component show_control(ControllerModel &controller_model, ConfigModel &config_model)
{
  using namespace ftxui;

  constexpr static int max_radius = static_cast<int>(
    static_cast<float>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5F * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;

  auto circle_area = Renderer([&controller_model, &config_model] {
    auto c = Canvas(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT);
    auto pos = controller_model.limited_block_position();

    c.DrawPointCircle(CONTROL_AREA_CENTER.x, CONTROL_AREA_CENTER.y, max_radius);
    c.DrawBlockCircleFilled(pos.x, pos.y, indicator_radius);

    if (config_model.debug) {
      c.DrawText(0, 0, fmt::format("ctrl x: {}", pos.x));
      c.DrawText(0, 4, fmt::format("ctrl y: {}", pos.y));// NOLINT Magic Number
      auto mouse_pos = controller_model.mouse_position;
      c.DrawText(0, 8, fmt::format("mouse x: {}", mouse_pos.x));// NOLINT Magic Number
      c.DrawText(0, 12, fmt::format("mouse y: {}", mouse_pos.y));// NOLINT Magic Number
      c.DrawText(0, 16, config_model.free_text);// NOLINT Magic Number
    }

    return canvas(std::move(c));
  });

    // gets not catched because it gets rendered
  return CatchEvent(circle_area, [&controller_model, &config_model](Event e) {
    const auto &mouse = e.mouse();

    config_model.free_text = fmt::format("is_mouse: {}, x: {}, y: {}", e.is_mouse(), mouse.x, mouse.y);

    if (e.is_mouse() && is_left_button_pressed(mouse)) {
      controller_model.mouse_position = { mouse.x * 2, mouse.y * 4 };
      return true;
    }

    controller_model.mouse_position = CONTROL_AREA_CENTER;

    return false;
  });
}

ftxui::Component
  add_key_events(ftxui::Component component, ConfigModel &config_model, const ftxui::ScreenInteractive::Callback &exit)
{
  return ftxui::CatchEvent(std::move(component), [&config_model, exit](const ftxui::Event &e) {
    if (e == ftxui::Event::Character('d')) {
      config_model.debug = !config_model.debug;
      return true;
    } else if (e == ftxui::Event::Character('q')) {
      exit();
      return true;
    }
    return false;
  });
}

int main()
{
  ControllerModel controller_model{
    CONTROL_AREA_CENTER,
    CONTROL_AREA_CENTER,
    static_cast<float>(CONTROL_AREA_WIDTH) * 0.5F * CONTROL_CIRCLE_FACTOR// NOLINT Magic Number
  };

  ConfigModel config_model{};
  MazeModel maze_model{};

  auto maze_control_renderer = ftxui::Renderer([&] {
    return ftxui::border(ftxui::hbox({ show_maze(controller_model, maze_model)->Render(),
      ftxui::separator(),
      show_control(controller_model, config_model)->Render() }));
  });

  //maze_control_renderer = ftxui::CatchEvent(maze_control_renderer, [&](const ftxui::Event &) { return false; });


  auto screen = ftxui::ScreenInteractive::FitComponent();

  auto all_renderer = add_key_events(maze_control_renderer, config_model, screen.ExitLoopClosure());

  screen.Loop(all_renderer);
}
