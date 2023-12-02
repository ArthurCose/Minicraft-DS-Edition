#include "instructionsmenu.h"

#include <nds.h>
#include <vector>
#include "titlemenu.h"
#include "../../gfx/screen.h"
#include "../../gfx/color.h"

void InstructionsMenu::tick(Game& game)
{
  if (game.justTapped(KEY_A) || game.justTapped(KEY_B)) {
    close(game);
  }
}

static std::vector<std::string> controlInstructions = {
    "Basic Controls:",
    "",
    " move with the D-Pad",
    " A to interact",
    " B to attack/use items",
    " X to open the inventory",
    " Y to unequip held item",
    " START to pause the game",
    " SELECT to move items",
    "",
    "Interact with an item in the",
    "inventory to equip it.",
};

static std::vector<std::string> winInstructions = {
    "Kill the air wizard",
    "to win the game!",
};

void InstructionsMenu::render(Screen& screen, Screen& bottomScreen)
{
  screen.clear(0);

  screen.renderTextCentered("HOW TO PLAY", screen.w / 2, 4 * 8, Color::get(0, 555, 555, 555));

  for (size_t i = 0; i < controlInstructions.size(); i++)
    screen.renderText(controlInstructions[i], 4, i * 8 + 6 * 8, Color::get(0, 333, 333, 333));

  for (size_t i = 0; i < winInstructions.size(); i++)
    screen.renderTextCentered(winInstructions[i], screen.w / 2, i * 8 + controlInstructions.size() * 8 + 8 * 8, Color::get(0, 333, 333, 333));
}
