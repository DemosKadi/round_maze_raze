#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "tilting_model.h"


// enum class MenuEntry { Level = 0, Random = 1, Quit = 2 };

/*
struct Position
{
  int x{};
  int y{};
};
 */

struct Vec2
{
  int x{};
  int y{};

  [[nodiscard]] Vec2 normalize() const { return *this; }
};

Position operator+(const Position &pos, const Vec2 &vec) { return { pos.x + vec.x, pos.y + vec.y }; }

void show_menu()
{
  using namespace ftxui;

  // std::array<std::string, 3> menu_entries{ "Level", "Random", "Quit" };
  MenuEntryOption entry_option{ .style_focused = [](Element elem) { return elem; } };

  std::vector<std::string> menu_entries{ "Level", "Random", "Quit" };

  auto screen = ScreenInteractive::TerminalOutput();
  MenuOption option;
  option.style_focused = [](Element elem) -> Element { return elem; };
  option.on_enter = screen.ExitLoopClosure();
  int menu_selected = 0;
  auto menu = Menu(&menu_entries, &menu_selected, &option);

  screen.Loop(menu);
  fmt::print("Selected: {}\n", menu_entries[static_cast<std::size_t>(menu_selected)]);
}

void show_example()
{
  using namespace ftxui;
  Element document = hbox({
    text("left") | border,
    text("middle") | border | flex,
    text("right") | border,
  });
  /*
  Element document = hbox({
    text("left"),
    text("middle") | flex,
    text("right"),
  });
   */

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);
  screen.Print();
}

template<typename T> inline T limit(T in, T limit_val) { return in > limit_val ? limit_val : in; }

template<typename T> inline T distance(T lhs, T rhs) { return static_cast<T>(std::sqrt(lhs * lhs + rhs * rhs)); }

void show_canvas()
{
  using namespace ftxui;

  constexpr static int width = 100;
  constexpr static int height = 100;
  constexpr static int max_radius = std::min(width, height);
  constexpr static Position center = { width / 2, height / 2 };

  // Position mouse_pos = center;

  TiltingModel tilting_model{ center, center, static_cast<double>(width) / 2.0 }; // NOLINT Magic Number

  auto circle_area = Renderer([&] {
    auto c = Canvas(width, height);
    c.DrawPointCircle(center.x, center.y, max_radius);
    auto mouse_pos = tilting_model.limited_mouse_position();
    c.DrawPointCircleFilled(mouse_pos.x, mouse_pos.y, 5);// NOLINT Magic Number

    return canvas(std::move(c));
  });

  // auto max_distance_to_center = distance(max_radius, max_radius);


  auto circle_with_mouse = CatchEvent(circle_area, [&](Event e) {
    // fmt::print(stderr, "is_mouse: {}\n", e.is_mouse());
    if (e.is_mouse()) {
      auto mouse = e.mouse();
      // fmt::print(stderr, "mouse.button: {}\n", mouse.button);
      // fmt::print(stderr, "mouse.motion: {}\n", mouse.motion);
      // fmt::print(stderr, "mouse.position: {} {}\n", mouse.x, mouse.y);
      if (mouse.button == Mouse::Button::Left && mouse.motion == Mouse::Motion::Pressed) {
        tilting_model.mouse_position({ (mouse.x - 1) * 2, (mouse.y - 1) * 4 });

        return false;
      }
    }

    tilting_model.mouse_position({ 50, 50 });// NOLINT Magic Numbers

    return false;
  });

  auto renderer = Renderer(circle_with_mouse, [&] { return circle_with_mouse->Render() | border; });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  /*
auto c = Canvas(100, 100); // NOLINT Magic Number
c.DrawPointCircleFilled(50, 50, 5); // NOLINT Magic Number

auto document = canvas(&c) | border;
auto screen = Screen::Create(Dimension::Fit(document));
Render(screen, document);
screen.Print();
   */
}

int main() { show_canvas(); }