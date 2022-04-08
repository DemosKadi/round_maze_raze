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

#include "../lib/config_model.h"
#include "../lib/controller_model.h"


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

std::shared_ptr<ftxui::ComponentBase> show_control(const std::shared_ptr<ControllerModel> &controller_model,
  const std::shared_ptr<ConfigModel> &config_model)
{
  using namespace ftxui;

  constexpr static int max_radius = static_cast<int>(
    static_cast<float>(std::min(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT)) * 0.5F * CONTROL_CIRCLE_FACTOR);
  constexpr static int indicator_radius = 5;

  auto circle_area = Renderer([controller_model, config_model] {
    auto c = Canvas(CONTROL_AREA_WIDTH, CONTROL_AREA_HEIGHT);
    auto pos = controller_model->limited_block_position();

    c.DrawPointCircle(CONTROL_AREA_CENTER.x, CONTROL_AREA_CENTER.y, max_radius);
    c.DrawBlockCircleFilled(pos.x, pos.y, indicator_radius);

    if (config_model->debug) {
      c.DrawText(0, 0, fmt::format("ctrl x: {}", pos.x));
      c.DrawText(0, 4, fmt::format("ctrl y: {}", pos.y));// NOLINT Magic Number
      auto mouse_pos = controller_model->mouse_position;
      c.DrawText(0, 8, fmt::format("mouse x: {}", mouse_pos.x));// NOLINT Magic Number
      c.DrawText(0, 12, fmt::format("mouse y: {}", mouse_pos.y));// NOLINT Magic Number
    }

    return canvas(std::move(c));
  });

  return CatchEvent(circle_area, [controller_model](Event e) {
    const auto &mouse = e.mouse();

    if (e.is_mouse() && is_left_button_pressed(mouse)) {
      controller_model->mouse_position = { mouse.x * 2, mouse.y * 4 };
      return true;
    }

    controller_model->mouse_position = CONTROL_AREA_CENTER;

    return false;
  });
}

std::shared_ptr<ftxui::ComponentBase> add_key_events(const std::shared_ptr<ftxui::ComponentBase> &element,
  const std::shared_ptr<ConfigModel> &config_model,
  ftxui::ScreenInteractive &screen)
{
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
  auto controller_model = std::make_shared<ControllerModel>(CONTROL_AREA_CENTER,
    CONTROL_AREA_CENTER,
    static_cast<float>(CONTROL_AREA_WIDTH) * 0.5F * CONTROL_CIRCLE_FACTOR);// NOLINT Magic Number

  auto config_model = std::make_shared<ConfigModel>();

  auto control_renderer = show_control(controller_model, config_model);

  auto screen = ftxui::ScreenInteractive::FitComponent();

  auto all_renderer = add_key_events(control_renderer, config_model, screen);

  screen.Loop(all_renderer);
}
