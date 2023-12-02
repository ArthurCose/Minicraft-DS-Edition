#include "deadmenu.h"

DeadMenu::DeadMenu(int time, int score)
  : ResultsMenu(time, score) {}

void DeadMenu::render(Screen& screen, Screen& bottomScreen)
{
  ResultsMenu::renderResults(screen, "You died! Aww!", "Press START to quit");
}
