#include "inventorymenu.h"

InventoryMenu::InventoryMenu(std::shared_ptr<Player> player) : player(player)
{
  // put held item back in inventory
  if (player->activeItem != NULL)
  {
    player->inventory.add(0, player->activeItem);
    player->activeItem = NULL;
  }
}

void InventoryMenu::tick(Game &game)
{
  if (game.justTapped(KEY_X) || game.justTapped(KEY_B) || game.justTapped(KEY_START))
    close(game);

  if (game.justTapped(KEY_UP))
    selected--;
  if (game.justTapped(KEY_DOWN))
    selected++;

  int len = player->inventory.items.size();

  if (len == 0)
    selected = 0;
  if (selected < 0)
    selected = len - 1;
  if (selected >= len)
    selected = 0;

  if (game.justTapped(KEY_A) && len > 0)
  {
    auto item = player->inventory.items[selected];
    player->inventory.removeItem(*item);
    player->activeItem = item;
    close(game);
  }
}

void InventoryMenu::render(Screen &screen, Screen &bottomScreen)
{
  screen.renderFrame("inventory", 1, 1, 12, 11);
  renderItemList(screen, 1, 1, 12, 11, player->inventory.items, selected);
}
