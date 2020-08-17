#include "deadmenu.h"

DeadMenu::DeadMenu(int time, int score)
    : ResultsMenu(time, score) {}

void DeadMenu::render(Screen &screen)
{
  ResultsMenu::render(screen);
  screen.renderText("You died! Aww!", 2 * 8, 4 * 8, Color::get(-1, 555, 555, 555));

  screen.renderText("Press START to quit", 2 * 8, 8 * 8, Color::get(-1, 333, 333, 333));
}
