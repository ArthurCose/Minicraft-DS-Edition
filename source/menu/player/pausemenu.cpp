#include "pausemenu.h"

#include "ingamemenu.h"

void PauseMenu::tick(Game &game)
{
  if (game.justTapped(KEY_START))
    close(game);
}

void PauseMenu::render(Screen &screen, Screen &bottomScreen)
{
  screen.renderTextCentered("PAUSED", screen.w / 2, screen.h / 2, Color::get(-1, 555, 555, 555));
}
