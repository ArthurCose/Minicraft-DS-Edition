#include "aboutmenu.h"

#include <nds.h>
#include "titlemenu.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"

void AboutMenu::tick(Game &game)
{
  if (game.justTapped(KEY_A) || game.justTapped(KEY_B))
  {
    close(game);
  }
}

static std::vector<std::string> aboutText = {
    "  MinicraftDS is a",
    "port of Minicraft",
    "by Arthur Cosentino",
    "",
    "  Minicraft was made",
    "by Markus Persson",
    "For the 22'nd ludum",
    "dare competition in",
    "december 2011.",
};

void AboutMenu::render(Screen &screen)
{
  screen.clear(0);

  screen.renderTextCentered("About MinicraftDS", screen.w / 2, 28, Color::get(0, 555, 555, 555));

  size_t widestLength = 0;

  for (size_t i = 0; i < aboutText.size(); i++)
  {
    if (widestLength < aboutText[i].size())
      widestLength = aboutText[i].size();
  }

  size_t leftPadding = (screen.w - widestLength * 8) / 2;

  for (size_t i = 0; i < aboutText.size(); i++)
    screen.renderText(aboutText[i], leftPadding, i * 8 + 6 * 8, Color::get(0, 333, 333, 333));
}
