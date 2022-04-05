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

#include "backend/config_model.h"
#include "backend/tilting_model.h"


constexpr int CONTROL_AREA_WIDTH = 100;
constexpr int CONTROL_AREA_HEIGHT = 100;
constexpr Position CONTROL_AREA_CENTER = { CONTROL_AREA_WIDTH / 2, CONTROL_AREA_HEIGHT / 2 };
constexpr double CONTROL_CIRCLE_FACTOR = 0.7;

std::shared_ptr<ftxui::ComponentBase> show_control(const std::shared_ptr<TiltingModel> &tilting_model,
  const std::shared_ptr<ConfigModel> &config_model)
{
  using namespace ftxui;

  constexpr static int max_radius = static_cast<int>(
    static_cast<double>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5 * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;

  auto circle_area = Renderer([tilting_model, config_model] {
    auto c = Canvas(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT);
    auto controller_pos = tilting_model->limited_mouse_position();
    c.DrawPointCircle(CONTROL_AREA_CENTER.x, CONTROL_AREA_CENTER.y, max_radius);
    c.DrawBlockCircleFilled(controller_pos.x, controller_pos.y, indicator_radius);

    if (config_model->debug) {
      c.DrawText(0, 0, fmt::format("ctrl x: {}", controller_pos.x));
      c.DrawText(0, 4, fmt::format("ctrl y: {}", controller_pos.y));// NOLINT Magic Number
      auto mouse_pos = tilting_model->mouse_position;
      c.DrawText(0, 8, fmt::format("mouse x: {}", mouse_pos.x));// NOLINT Magic Number
      c.DrawText(0, 12, fmt::format("mouse y: {}", mouse_pos.y));// NOLINT Magic Number
    }

    return canvas(std::move(c));
  });

  return CatchEvent(circle_area, [tilting_model](Event e) {
    if (e.is_mouse()) {
      if (auto mouse = e.mouse(); mouse.button == Mouse::Button::Left && mouse.motion == Mouse::Motion::Pressed) {
        tilting_model->mouse_position = { mouse.x * 2, mouse.y * 4 };
        return true;
      }
    }

    tilting_model->mouse_position = CONTROL_AREA_CENTER;

    return false;
  });
}

std::shared_ptr<ftxui::ComponentBase> add_key_events(const std::shared_ptr<ftxui::ComponentBase>& element, const std::shared_ptr<ConfigModel>& config_model, ftxui::ScreenInteractive &screen) {
  return ftxui::CatchEvent(element, [config_model, &screen](const ftxui::Event &e) {
    if (e.is_character()) {
      if (e.character() == "d") {
        config_model->debug = !config_model->debug;
        return true;
      } else if (e.character() == "q") {
        screen.ExitLoopClosure()();
        return true;
      }
    }
    return false;
  });
}

int main()
{
  auto tilting_model = std::make_shared<TiltingModel>(CONTROL_AREA_CENTER,
    CONTROL_AREA_CENTER,
    static_cast<double>(CONTROL_AREA_WIDTH) * 0.5 * CONTROL_CIRCLE_FACTOR);// NOLINT Magic Number

  auto config_model = std::make_shared<ConfigModel>();

  auto control_renderer = show_control(tilting_model, config_model);

  auto screen = ftxui::ScreenInteractive::FitComponent();

  auto all_renderer = add_key_events(control_renderer, config_model, screen);

  screen.Loop(all_renderer);
}
