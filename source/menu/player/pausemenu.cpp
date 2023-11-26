#include "pausemenu.h"

#include "ingamemenu.h"
#include "../main/titlemenu.h"
#include "../keyboardmenu.h"

static const std::array<std::string_view, 3> options = { "SAVE", "QUIT", "CANCEL" };

void PauseMenu::tick(Game& game)
{
  game.frameSkipEnabled = false;

  if (saving) {
    game.save();
    saving = false;
  }

  if (game.justTapped(KEY_START) || game.justTapped(KEY_B)) {
    close(game);
  }

  if (game.justTappedOrRepeat(KEY_UP)) {
    selected--;
  }
  if (game.justTappedOrRepeat(KEY_DOWN)) {
    selected++;
  }

  if (selected < 0) {
    selected = (int)options.size() - 1;
  }
  if (selected >= (int)options.size()) {
    selected = 0;
  }

  if (game.justTapped(KEY_A)) {
    switch (selected) {
    case 0:
      // queue save
      if (game.worldName.empty()) {
        // set world name
        auto menu = std::make_unique<KeyboardMenu>([this, &game](std::string name) {
          game.worldName = name;
          game.resolveNameConflict();
          saving = true;
        });

        menu->setPlaceholder("World Name");
        menu->setCharacterLimit(16);
        game.enterMenu(std::move(menu));
      } else {
        // straight to saving
        saving = true;
      }
      break;
    case 1:
      // quit
      game.resetGame();
      game.setMenu(std::make_unique<TitleMenu>());
      break;
    case 2:
      // cancel
      close(game);
      break;
    }
  }
}

void PauseMenu::render(Screen& screen, Screen& bottomScreen)
{
  bottomScreen.clear(Color::get(0));

  int horizontalTiles = bottomScreen.w / 8;
  int verticalTiles = bottomScreen.h / 8;

  int width = 7;
  int height = 4;
  int marginX = (horizontalTiles - width) / 2;
  int marginY = verticalTiles / 2 + 2;
  screen.renderFrame("PAUSED", marginX, marginY, marginX + width, marginY + height);

  int textColor = Color::get(5, 555, 555, 555);

  for (int i = 0; i < (int)options.size(); i++) {
    int x = marginX * 8;
    int y = (marginY + i + 1) * 8;
    screen.renderText(options[i], x + 8, y, textColor);

    if (i == selected) {
      screen.renderText(">", x, y, textColor);
      screen.renderText("<", x + width * 8, y, textColor);
    }
  }

  if (saving) {
    screen.renderTextCentered("SAVING...", screen.w / 2, screen.h - 16, Color::get(-1, 555, 555, 555));
  }
}
