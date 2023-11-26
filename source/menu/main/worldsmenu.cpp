#include "worldsmenu.h"

#include <string_view>
#include <string>
#include <filesystem>
#include <nds.h>
#include "titlemenu.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"
#include "../player/ingamemenu.h"
#include "../keyboardmenu.h"
#include "deletemenu.h"

const int PAGE_LEN = 10;

namespace fs = std::filesystem;

WorldsMenu::WorldsMenu() {
  if (fs::exists(Game::SAVE_FOLDER)) {
    std::vector<fs::directory_entry> validEntries;

    // find save files
    for (auto& entry : fs::directory_iterator(Game::SAVE_FOLDER)) {
      if (entry.is_directory()) {
        continue;
      }

      if (entry.path().extension() != ".dat") {
        continue;
      }

      validEntries.push_back(entry);
    }

    // sort entries, putting recently played/saved at the top
    std::sort(validEntries.begin(), validEntries.end(), [](auto& a, auto& b) {
      return a.last_write_time() > b.last_write_time();
    });

    // add entries to the viewable list
    for (auto& entry : validEntries) {
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

  if (worldNames.empty()) {
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

  if (game.justTapped(KEY_A)) {
    // load world
    Sound::test.play();
    game.load(worldNames[selected]);
    game.setMenu(std::make_unique<InGameMenu>(game.player, game.levels[game.currentLevel].map));
    return;
  }

  if (game.justTapped(KEY_Y)) {
    // rename world
    auto menu = std::make_unique<KeyboardMenu>([this, &game](std::string name) {
      if (name == worldNames[selected]) {
        // the name was not modified
        return;
      }

      game.worldName = name;
      game.resolveNameConflict();

      fs::rename(Game::savePath(worldNames[selected]), Game::savePath(game.worldName));

      worldNames[selected] = game.worldName;
    });

    menu->setPlaceholder("World Name");
    menu->setText(worldNames[selected]);
    menu->setCharacterLimit(16);
    game.enterMenu(std::move(menu));
    return;
  }

  if (game.justTapped(KEY_X)) {
    // delete world
    auto menu = std::make_unique<DeleteMenu>(worldNames[selected], [this](bool yes) {
      if (!yes) {
        return;
      }

      fs::remove(Game::savePath(worldNames[selected]));
      worldNames.erase(worldNames.begin() + selected);

      if (selected > 0) {
        selected--;
      }
    });

    game.enterMenu(std::move(menu));
    return;
  }
}

void WorldsMenu::render(Screen& screen, Screen& bottomScreen)
{
  screen.clear(0);
  bottomScreen.clear(0);

  const int OPTION_TEXT_COLOR = Color::get(0, 333, 333, 333);
  const int HIGHLIGHT_COLOR = Color::get(0, 555, 555, 555);

  int centerX = screen.w / 2;

  auto renderWorldName = [&screen, centerX](std::string_view name, int color, int i) {
    screen.renderTextCentered(name, centerX, i * 8 + 7 * 8, color);
  };

  // render title
  renderWorldName("Saved Worlds", HIGHLIGHT_COLOR, -2);

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

  // render help
  screen.renderTextCentered("(Y)rename        (X)delete", centerX, screen.h - 8, OPTION_TEXT_COLOR);
}
