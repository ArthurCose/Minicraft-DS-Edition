#include "ingamemenu.h"

InGameMenu::InGameMenu(std::shared_ptr<Player> player, std::shared_ptr<std::vector<unsigned char>> map)
{
  this->player = player;
  this->map = map;
  blocksGameTick = false;
}

void InGameMenu::tick(Game &game)
{
}

void InGameMenu::render(Screen &screen, Screen &bottomScreen)
{
  bottomScreen.clear(Color::get(5));

  renderHud(screen);
  renderInventory(bottomScreen);
  renderMap(bottomScreen);
}

void InGameMenu::renderHud(Screen &screen)
{
  const int hudTop = screen.h - 8;
  const int staminaBarLeft = screen.w - 10 * 8;

  for (int i = 0; i < 10; i++)
  {
    if (i < player->health)
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 200, 500, 533), 0);
    else
      screen.renderTile(i * 8, hudTop, 0 + 12 * 32, Color::get(000, 100, 000, 000), 0);

    if (player->staminaRechargeDelay > 0)
    {
      if (player->staminaRechargeDelay / 4 % 2 == 0)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 555, 000, 000), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    }
    else
    {
      if (i < player->stamina)
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 220, 550, 553), 0);
      else
        screen.renderTile(staminaBarLeft + i * 8, hudTop, 1 + 12 * 32, Color::get(000, 110, 000, 000), 0);
    }
  }
}

void InGameMenu::renderInventory(Screen &bottomScreen)
{
  auto &items = player->inventory.items;

  int inventoryY = 8;
  bottomScreen.renderText("L", 8, inventoryY, Color::get(-1, 555, 555, 555));
  bottomScreen.renderText("R", bottomScreen.w - 16, inventoryY, Color::get(-1, 555, 555, 555));

  int itemCount = items.size();
  int itemRenderCount = (bottomScreen.w - 48) / 16;
  int itemStart = player->activeItemIndex - itemRenderCount / 2;

  if (itemStart + itemRenderCount >= itemCount)
    itemStart = itemCount - itemRenderCount;
  if (itemStart < 0)
    itemStart = 0;

  for (int i = 0; i < itemRenderCount; i++)
  {
    int index = itemStart + i;

    if (index >= itemCount)
      continue;

    // 16 for L padding, 8 for L, 4 to center
    int x = i * 16 + 16 + 8 + 4;

    if (player->itemSelected && player->activeItemIndex == index)
      bottomScreen.renderBox(x, inventoryY, 8, 8, Color::get(444));

    auto &item = *items[index];

    bottomScreen.renderTile(x, inventoryY, item.getSprite(), item.getColor(), 0);
  }

  if (player->itemSelected)
  {
    auto activeItem = player->getActiveItem();
    auto name = activeItem->getName();

    int iconOffset = -12;
    int itemNameLeft = (bottomScreen.w - name.size() * 8) / 2 - iconOffset / 2;
    int itemNameTop = bottomScreen.h - 24;

    bottomScreen.renderTile(itemNameLeft + iconOffset, itemNameTop, activeItem->getSprite(), activeItem->getColor(), 0);
    bottomScreen.renderText(name, itemNameLeft, itemNameTop, Color::get(-1, 555, 555, 555));

    if (auto resourceItem = std::dynamic_pointer_cast<ResourceItem>(activeItem))
      bottomScreen.renderTextCentered(std::to_string(resourceItem->count), bottomScreen.w / 2, itemNameTop + 12, Color::get(-1, 222, 222, 222));
  }
}

void InGameMenu::renderMap(Screen &screen)
{
  int mapLeft = 64;
  int mapTop = 32;
  int levelSize = 128;

  if (auto sscreen = dynamic_cast<SoftwareScreen *>(&screen))
  {
    for (int y = 0; y < levelSize; y++)
    {
      int channel = y % 4;
      // wait for channel
      while (dmaBusy(channel))
        ;

      dmaCopyWordsAsynch(
          channel,
          &map->at(y * levelSize),
          &sscreen->pixels[(y + mapTop) * screen.w + mapLeft],
          levelSize);
    }
  }
  else
  {
    for (int y = 0; y < levelSize; y++)
    {
      for (int x = 0; x < levelSize; x++)
      {
        screen.renderPixel(mapLeft + x, mapTop + y, map->at(y * levelSize + x));
      }
    }
  }

  screen.setOffset(
      player->x - mapLeft - (player->x >> 4),
      player->y - mapTop - (player->y >> 4));
  player->render(screen);
  screen.setOffset(0, 0);
}
