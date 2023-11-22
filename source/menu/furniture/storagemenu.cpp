#include "storagemenu.h"

StorageMenu::StorageMenu(std::string title, std::shared_ptr<Inventory> storageInventory, std::shared_ptr<Player> player)
{
  this->title = title;
  this->storageInventory = storageInventory;
  this->player = player;
}

void StorageMenu::tick(Game& game)
{
  if (game.justTapped(KEY_B) || game.justTapped(KEY_X) || game.justTapped(KEY_START))
    close(game);

  if (game.justTapped(KEY_LEFT) || game.justTapped(KEY_L)) {
    window = 0;
    int tmp = selected;
    selected = oSelected;
    oSelected = tmp;
    moving = false;
  }
  if (game.justTapped(KEY_RIGHT) || game.justTapped(KEY_R)) {
    window = 1;
    int tmp = selected;
    selected = oSelected;
    oSelected = tmp;
    moving = false;
  }

  Inventory& i = window == 1 ? player->inventory : *storageInventory;
  Inventory& i2 = window == 0 ? player->inventory : *storageInventory;

  int len = i.items.size();

  if (selected < 0)
    selected = 0;
  if (selected >= len)
    selected = len - 1;

  int lastPosition = selected;

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

  if (moving && lastPosition != selected) {
    auto start = i.items.begin();
    std::iter_swap(start + lastPosition, start + selected);
  }

  if (game.justTapped(KEY_SELECT)) {
    moving = !moving;

    if (moving)
      blinkTimer = 15;
  }

  if (moving && (game.justTapped(KEY_A) || game.justTapped(KEY_B))) {
    moving = false;
  } else if (game.justTapped(KEY_A) && len > 0) {
    auto item = i.items[selected];

    i.removeItem(*item);
    i2.add(oSelected, item);

    len = i.items.size();

    if (selected >= len)
      selected = len - 1;
  }

  if (moving)
    blinkTimer = (blinkTimer + 1) % 30;
  else
    blinkTimer = 0;
}

void StorageMenu::render(Screen& screen, Screen& bottomScreen)
{
  if (window == 1)
    screen.setOffset(6 * 8, 0);

  renderInventory(screen, 1, 1, title, storageInventory->items, window == 0);
  renderInventory(screen, 13, 1, "inventory", player->inventory.items, window == 1);

  screen.setOffset(0, 0);
}

void StorageMenu::renderInventory(Screen& screen, int x, int y, std::string name, std::vector<std::shared_ptr<Item>>& items, bool active)
{
  int currentSelection = active ? selected : oSelected;

  screen.renderFrame(name, x, y, x + 11, y + 11);
  renderItemList(screen, x, y, x + 11, y + 11, items, currentSelection, !active || blinkTimer < 15);
}