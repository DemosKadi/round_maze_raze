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

struct DrawArea
{
  int x{};
  int y{};
};

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


void draw_maze(ftxui::Canvas &canvas, DrawArea draw_area, const MazeModel &maze_model)
{
  constexpr static int BALL_RADIUS = 3;

  const auto &ball_pos = maze_model.ball.position.rounded();
  auto x = ball_pos.x + draw_area.x;
  auto y = ball_pos.y + draw_area.y;
  canvas.DrawPointCircleFilled(x, y, BALL_RADIUS);
}

void draw_control_area(ftxui::Canvas &canvas,
  DrawArea draw_area,
  const ControllerModel &controller_model,
  const ConfigModel &config_model)
{
  constexpr static int max_radius = static_cast<int>(
    static_cast<float>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5F * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;
  auto pos = controller_model.limited_block_position();

  auto x_off = [&draw_area](int x) { return draw_area.x + x; };
  auto y_off = [&draw_area](int y) { return draw_area.y + y; };

  canvas.DrawPointCircle(x_off(CONTROL_AREA_CENTER.x), y_off(CONTROL_AREA_CENTER.y), max_radius);
  canvas.DrawBlockCircleFilled(x_off(pos.x), y_off(pos.y), indicator_radius);

  if (config_model.debug) {
    canvas.DrawText(0, 0, fmt::format("ctrl x: {}", pos.x));
    canvas.DrawText(0, 4, fmt::format("ctrl y: {}", pos.y));// NOLINT Magic Number
    auto mouse_pos = controller_model.mouse_position;
    canvas.DrawText(0, 8, fmt::format("mouse x: {}", mouse_pos.x));// NOLINT Magic Number
    canvas.DrawText(0, 12, fmt::format("mouse y: {}", mouse_pos.y));// NOLINT Magic Number
    canvas.DrawText(0, 16, config_model.free_text);// NOLINT Magic Number
  }
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

  constexpr static DrawArea maze_area{ 0, 0 };
  constexpr static DrawArea control_area{ CONTROL_AREA_WIDTH, 0 };

  auto maze_control_renderer = ftxui::Renderer([&] {
    ftxui::Canvas canvas{ CONTROL_AREA_WIDTH * 2, CONTROL_AREA_HEIGHT };
    draw_maze(canvas, maze_area, maze_model);
    draw_control_area(canvas, control_area, controller_model, config_model);

    return ftxui::canvas(std::move(canvas));
  });

  auto screen = ftxui::ScreenInteractive::FitComponent();
  maze_control_renderer = ftxui::CatchEvent(maze_control_renderer, [&](ftxui::Event e) {
    if (e.is_character()) {
      if (e == ftxui::Event::Character('d')) {
        config_model.debug = !config_model.debug;
        return true;
      } else if (e == ftxui::Event::Character('q')) {
        screen.ExitLoopClosure()();
        return true;
      }
    } else if (e.is_mouse()) {
      const auto &mouse = e.mouse();

      config_model.free_text = fmt::format("is_mouse: {}, x: {}, y: {}", e.is_mouse(), mouse.x, mouse.y);

      if (is_left_button_pressed(mouse)) {
        controller_model.mouse_position = { mouse.x * 2 - control_area.x, mouse.y * 4 - control_area.y };
        return true;
      }

      controller_model.mouse_position = CONTROL_AREA_CENTER;
    }

    return false;
  });

  screen.Loop(maze_control_renderer);
}
