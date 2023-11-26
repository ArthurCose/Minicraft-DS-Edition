#include "deletemenu.h"

#include <nds.h>
#include "../../gfx/screen.h"
#include "../../gfx/color.h"
#include "../player/ingamemenu.h"


DeleteMenu::DeleteMenu(std::string name, std::function<void(bool)> callback) :
  callback(callback)
{
  title = "Delete " + name + "?";
}

void DeleteMenu::tick(Game& game)
{
  if (game.justTapped(KEY_B)) {
    close(game);
    return;
  }

  if (game.justTappedOrRepeat(KEY_UP)) {
    yes = !yes;
  }
  if (game.justTappedOrRepeat(KEY_DOWN)) {
    yes = !yes;
  }

  if (game.justTapped(KEY_A)) {
    callback(yes);
    close(game);
    return;
  }
}

void DeleteMenu::render(Screen& screen, Screen& bottomScreen)
{
  screen.clear(0);

  const int TITLE_Y = 8 * 9;
  const int OPTIONS_Y = TITLE_Y + 8 * 3;

  screen.renderTextCentered(title, screen.w / 2, TITLE_Y, Color::get(0, 555, 555, 555));

  const int OPTION_TEXT_COLOR = Color::get(0, 333, 333, 333);
  const int HIGHLIGHT_COLOR = Color::get(0, 555, 555, 555);

  std::string_view yesText = "YES";
  int yesColor = OPTION_TEXT_COLOR;
  std::string_view noText = "NO";
  int noColor = OPTION_TEXT_COLOR;

  if (yes) {
    yesText = "> YES <";
    yesColor = HIGHLIGHT_COLOR;
  } else {
    noText = "> NO <";
    noColor = HIGHLIGHT_COLOR;
  }

  screen.renderTextCentered(yesText, screen.w / 2, OPTIONS_Y, yesColor);
  screen.renderTextCentered(noText, screen.w / 2, OPTIONS_Y + 8, noColor);
}
