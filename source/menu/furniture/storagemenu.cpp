#include "storagemenu.h"

StorageMenu::StorageMenu(std::string title, std::shared_ptr<Inventory> storageInventory, std::shared_ptr<Player> player)
{
  this->title = title;
  this->storageInventory = storageInventory;
  this->player = player;
}

void StorageMenu::tick(Game &game)
{
  if (game.justTapped(KEY_B) || game.justTapped(KEY_B) || game.justTapped(KEY_START))
    close(game);

  if (game.justTapped(KEY_LEFT) || game.justTapped(KEY_L))
  {
    window = 0;
    int tmp = selected;
    selected = oSelected;
    oSelected = tmp;
  }
  if (game.justTapped(KEY_RIGHT) || game.justTapped(KEY_R))
  {
    window = 1;
    int tmp = selected;
    selected = oSelected;
    oSelected = tmp;
  }

  Inventory &i = window == 1 ? player->inventory : *storageInventory;
  Inventory &i2 = window == 0 ? player->inventory : *storageInventory;

  int len = i.items.size();

  if (selected < 0)
    selected = 0;
  if (selected >= len)
    selected = len - 1;

  if (game.justTapped(KEY_UP))
    selected--;
  if (game.justTapped(KEY_DOWN))
    selected++;

  if (len == 0)
    selected = 0;
  if (selected < 0)
    selected = len - 1;
  if (selected >= len)
    selected = 0;

  if (game.justTapped(KEY_A) && len > 0)
  {
    auto item = i.items[selected];

    i.removeItem(*item);
    i2.add(oSelected, item);

    len = i.items.size();

    if (selected >= len)
      selected = len - 1;
  }
}

void StorageMenu::render(Screen &screen)
{
  if (window == 1)
    screen.setOffset(6 * 8, 0);

  screen.renderFrame(title, 1, 1, 12, 11);
  renderItemList(screen, 1, 1, 12, 11, storageInventory->items, window == 0 ? selected : oSelected);

  screen.renderFrame("inventory", 13, 1, 13 + 11, 11);
  renderItemList(screen, 13, 1, 13 + 11, 11, player->inventory.items, window == 1 ? selected : oSelected);
  screen.setOffset(0, 0);
}
