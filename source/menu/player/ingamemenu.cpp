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

  if (player->activeItem != NULL)
  {
    auto &activeItem = *player->activeItem;
    auto name = activeItem.getName();

    int iconOffset = -12;
    int itemNameLeft = (screen.w - name.size() * 8) / 2 - iconOffset / 2;
    int itemNameTop = screen.h - 16;

    screen.renderTile(itemNameLeft + iconOffset, itemNameTop, activeItem.getSprite(), activeItem.getColor(), 0);
    screen.renderText(name, itemNameLeft, itemNameTop, Color::get(-1, 555, 555, 555));
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
