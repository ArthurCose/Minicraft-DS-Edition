#include "instructionsmenu.h"

#include <nds.h>
#include <vector>
#include "titlemenu.h"
#include "../gfx/screen.h"
#include "../gfx/color.h"

void InstructionsMenu::tick(Game &game)
{
  if (game.justTapped(KEY_A))
  {
    close(game);
  }
}

static std::vector<std::string> controlInstructions = {
    "Controls:",
    " move with the D-Pad",
    " A to use items",
    " B to attack",
    " X to open the inventory",
    "",
    "Select an item in the inventory to",
    "equip it.",
};

static std::vector<std::string> winInstructions = {
    "Kill the air wizard",
    "to win the game!",
};

void InstructionsMenu::render(Screen &screen)
{
  screen.clear(0);

  int topPadding = (screen.h - 8 - 16 - controlInstructions.size() * 8 - 24 - winInstructions.size() * 8) / 2;

  screen.renderTextCentered("HOW TO PLAY", screen.w / 2, topPadding + 4, Color::get(0, 555, 555, 555));

  for (size_t i = 0; i < controlInstructions.size(); i++)
    screen.renderText(controlInstructions[i], 4, i * 8 + topPadding + 2 * 8, Color::get(0, 333, 333, 333));

  for (size_t i = 0; i < winInstructions.size(); i++)
    screen.renderTextCentered(winInstructions[i], screen.w / 2, i * 8 + controlInstructions.size() * 8 + topPadding + 5 * 8, Color::get(0, 333, 333, 333));
}
