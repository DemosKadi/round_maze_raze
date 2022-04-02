#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <fmt/core.h>

#include <string>
#include <vector>

// enum class MenuEntry { Level = 0, Random = 1, Quit = 2 };

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

int main() {}