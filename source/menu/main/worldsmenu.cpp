#include "worldsmenu.h"

#include <string_view>
#include <string>
#include <filesystem>
#include <nds.h>
#include "titlemenu.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"
#include "../player/ingamemenu.h"

const int PAGE_LEN = 10;

namespace fs = std::filesystem;

WorldsMenu::WorldsMenu() {
  if (fs::exists(Game::SAVE_FOLDER)) {
    for (auto& entry : fs::directory_iterator(Game::SAVE_FOLDER)) {
      if (entry.is_directory()) {
        continue;
      }

      if (entry.path().extension() != ".dat") {
        continue;
      }

      auto name = entry.path().stem().string();

      worldNames.push_back(name);
    }
  }
}

void WorldsMenu::tick(Game& game)
{
  if (game.justTapped(KEY_B)) {
    close(game);
    return;
  }

  if (game.justTappedOrRepeat(KEY_UP)) {
    selected--;
  }
  if (game.justTappedOrRepeat(KEY_DOWN)) {
    selected++;
  }

  int optionCount = worldNames.size();

  if (selected < 0) {
    selected = optionCount - 1;
  }
  if (selected >= optionCount) {
    selected = 0;
  }

  if (game.justTapped(KEY_A) && optionCount > 0) {
    game.load(worldNames[selected]);
    game.frameSkipEnabled = true;
    game.setMenu(std::make_unique<InGameMenu>(game.player, game.levels[game.currentLevel].map));
  }
}

void WorldsMenu::render(Screen& screen, Screen& bottomScreen)
{
  screen.clear(0);

  screen.renderTextCentered("Saved Worlds", screen.w / 2, 28, Color::get(0, 555, 555, 555));

  const int OPTION_TEXT_COLOR = Color::get(0, 333, 333, 333);
  const int HIGHLIGHT_COLOR = Color::get(0, 555, 555, 555);
  const int ARROW_COLOR = Color::get(0, 551, 551, 551);

  int centerX = screen.w / 2;

  auto renderWorldName = [&screen, centerX](std::string_view name, int color, int i) {
    screen.renderTextCentered(name, centerX, i * 8 + 7 * 8, color);
  };

  if (worldNames.empty()) {
    screen.renderTextCentered("No Saved Worlds", centerX, screen.h / 2 - 4, OPTION_TEXT_COLOR);
    return;
  }

  int upperLimit = std::min((int)worldNames.size(), PAGE_LEN);
  int topIndex = std::clamp(selected - PAGE_LEN / 2, 0, (int)worldNames.size() - PAGE_LEN);

  for (int i = 0; i < upperLimit; i++) {
    int index = topIndex + i;

    auto worldName = worldNames[index];
    auto color = OPTION_TEXT_COLOR;

    if (index == selected) {
      color = HIGHLIGHT_COLOR;
      worldName = "> " + worldName + " <";
    }

    renderWorldName(worldName, color, i + 2);
  }
}
