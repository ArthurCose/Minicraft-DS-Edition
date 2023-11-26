#include "inventorymenu.h"

#include <algorithm>

InventoryMenu::InventoryMenu(std::shared_ptr<Player> player) : player(player)
{
  selected = player->getSelectedItemIndex();
  player->setItemHeld(false);
}

void InventoryMenu::tick(Game& game)
{
  if (game.justTapped(KEY_X) || game.justTapped(KEY_B) || game.justTapped(KEY_START))
    close(game);

  int lastPosition = selected;

  if (game.justTappedOrRepeat(KEY_UP))
    selected--;
  if (game.justTappedOrRepeat(KEY_DOWN))
    selected++;

  int len = player->inventory.items.size();

  if (len == 0)
    selected = 0;
  if (selected < 0)
    selected = len - 1;
  if (selected >= len)
    selected = 0;

  if (moving && lastPosition != selected) {
    player->inventory.swapSlots(lastPosition, selected);
  }

  if (game.justTapped(KEY_SELECT)) {
    moving = !moving;

    if (moving)
      blinkTimer = 15;
  }

  if (moving && (game.justTapped(KEY_A) || game.justTapped(KEY_B))) {
    moving = false;
  } else if (game.justTapped(KEY_A) && len > 0) {
    player->setItemHeld(true);
    player->setSelectedItemIndex(selected);
    close(game);
  }

  if (moving)
    blinkTimer = (blinkTimer + 1) % 30;
  else
    blinkTimer = 0;
}

void InventoryMenu::render(Screen& screen, Screen& bottomScreen)
{
  screen.renderFrame("inventory", 1, 1, 12, 11);
  renderItemList(screen, 1, 1, 12, 11, player->inventory.items, selected, blinkTimer < 15);
}
