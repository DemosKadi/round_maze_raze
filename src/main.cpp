#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "backend/tilting_model.h"


constexpr int CONTROL_AREA_WIDTH = 100;
constexpr int CONTROL_AREA_HEIGHT = 100;
constexpr Position CONTROL_AREA_CENTER = { CONTROL_AREA_WIDTH / 2, CONTROL_AREA_HEIGHT / 2 };
constexpr double CONTROL_CIRCLE_FACTOR = 0.7;

std::shared_ptr<ftxui::ComponentBase> show_control(const std::shared_ptr<TiltingModel> &tilting_model)
{
  using namespace ftxui;

  constexpr static int max_radius = static_cast<int>(
    static_cast<double>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5 * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;

  auto circle_area = Renderer([tilting_model] {
    auto c = Canvas(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT);
    auto mouse_pos = tilting_model->limited_mouse_position();
    c.DrawPointCircle(CONTROL_AREA_CENTER.x, CONTROL_AREA_CENTER.y, max_radius);
    c.DrawBlockCircleFilled(mouse_pos.x, mouse_pos.y, indicator_radius);
    c.DrawText(0, 0, fmt::format("x: {}", mouse_pos.x));
    c.DrawText(0, 4, fmt::format("y: {}", mouse_pos.y));// NOLINT Magic Number

    return canvas(std::move(c));
  });

  return CatchEvent(circle_area, [tilting_model](Event e) {
    if (e.is_mouse()) {
      if (auto mouse = e.mouse(); mouse.button == Mouse::Button::Left && mouse.motion == Mouse::Motion::Pressed) {
        tilting_model->mouse_position({ (mouse.x - 1) * 2, (mouse.y - 1) * 4 });
        return false;
      }
    }

    tilting_model->mouse_position(CONTROL_AREA_CENTER);

    return false;
  });
}

int main()
{
  auto tilting_model = std::make_shared<TiltingModel>(CONTROL_AREA_CENTER,
    CONTROL_AREA_CENTER,
    static_cast<double>(CONTROL_AREA_WIDTH) * 0.5 * CONTROL_CIRCLE_FACTOR);// NOLINT Magic Number

  auto control_renderer = show_control(tilting_model);

  auto screen = ftxui::ScreenInteractive::FitComponent();
  screen.Loop(control_renderer);
}