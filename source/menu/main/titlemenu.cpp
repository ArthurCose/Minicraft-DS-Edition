#include "titlemenu.h"

#include <vector>
#include <string>
#include <string_view>
#include <nds.h>
#include "../../gfx/color.h"
#include "../../sound/sound.h"
#include "../world/generatemenu.h"
#include "worldsmenu.h"
#include "instructionsmenu.h"
#include "aboutmenu.h"

static std::vector<std::string_view> options = {
  "New game",
  "Saved Worlds",
  "How to play",
  "About",
  "Quit"
};

void TitleMenu::tick(Game& game)
{
  game.frameSkipEnabled = false;

  if (game.justTappedOrRepeat(KEY_UP))
    selected--;
  if (game.justTappedOrRepeat(KEY_DOWN))
    selected++;

  int optionCount = options.size();

  if (selected < 0)
    selected = optionCount - 1;
  if (selected >= optionCount)
    selected = 0;

  if (game.justTapped(KEY_A)) {
    switch (selected) {
    case 0:
      Sound::test.play();
      game.resetGame();
      game.setMenu(std::make_unique<GenerateMenu>());
      break;
    case 1:
      game.enterMenu(std::make_unique<WorldsMenu>());
      break;
    case 2:
      game.enterMenu(std::make_unique<InstructionsMenu>());
      break;
    case 3:
      game.enterMenu(std::make_unique<AboutMenu>());
      break;
    case 4:
      exit(0);
      break;
    }
  }
}

void TitleMenu::render(Screen& screen, Screen& bottomScreen)
{
  if (!clearedSubScreen) {
    bottomScreen.clear(0);
    clearedSubScreen = true;
  }

  screen.clear(0);

  int h = 2;
  int w = 13;
  int titleColor = Color::get(0, 10, 131, 551);
  int xo = (screen.w - w * 8) / 2;
  int yo = 24;
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      screen.renderTile(xo + x * 8, yo + y * 8, x + (y + 6) * 32, titleColor, 0);
    }
  }

  int subtitleColor = Color::get(0, 551, 551, 551);
  screen.renderTextCentered("DS Edition!", screen.w / 2, 6 * 8, subtitleColor);

  int optionColor = Color::get(0, 222, 222, 222);
  int highlightColor = Color::get(0, 555, 555, 555);

  for (int i = 0; i < (int)options.size(); i++) {
    std::string msg = std::string(options[i]);

    int col = optionColor;
    if (i == selected) {
      msg = "> " + msg + " <";
      col = highlightColor;
    }

    screen.renderTextCentered(msg, screen.w / 2, (10 + i) * 8 + 4, col);
  }
}
