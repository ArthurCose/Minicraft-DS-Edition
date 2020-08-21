#include "cloudtile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

CloudTile::CloudTile(int id)
    : Tile(id) {}

int CloudTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(444);
}

void CloudTile::render(Screen &screen, Level &level, int x, int y)
{
  int col = Color::get(444, 444, 555, 555);
  int transitionColor = Color::get(333, 444, 555, 20);

  bool u = level.getTile(x, y - 1) == Tile::infiniteFall;
  bool d = level.getTile(x, y + 1) == Tile::infiniteFall;
  bool l = level.getTile(x - 1, y) == Tile::infiniteFall;
  bool r = level.getTile(x + 1, y) == Tile::infiniteFall;

  bool ul = level.getTile(x - 1, y - 1) == Tile::infiniteFall;
  bool dl = level.getTile(x - 1, y + 1) == Tile::infiniteFall;
  bool ur = level.getTile(x + 1, y - 1) == Tile::infiniteFall;
  bool dr = level.getTile(x + 1, y + 1) == Tile::infiniteFall;

  if (!u && !l)
  {
    if (!ul)
      screen.renderTile(x * 16 + 0, y * 16 + 0, 17, col, 0);
    else
      screen.renderTile(x * 16 + 0, y * 16 + 0, 7 + 0 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (l ? 6 : 5) + (u ? 2 : 1) * 32, transitionColor, 3);

  if (!u && !r)
  {
    if (!ur)
      screen.renderTile(x * 16 + 8, y * 16 + 0, 18, col, 0);
    else
      screen.renderTile(x * 16 + 8, y * 16 + 0, 8 + 0 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (r ? 4 : 5) + (u ? 2 : 1) * 32, transitionColor, 3);

  if (!d && !l)
  {
    if (!dl)
      screen.renderTile(x * 16 + 0, y * 16 + 8, 20, col, 0);
    else
      screen.renderTile(x * 16 + 0, y * 16 + 8, 7 + 1 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (l ? 6 : 5) + (d ? 0 : 1) * 32, transitionColor, 3);
  if (!d && !r)
  {
    if (!dr)
      screen.renderTile(x * 16 + 8, y * 16 + 8, 19, col, 0);
    else
      screen.renderTile(x * 16 + 8, y * 16 + 8, 8 + 1 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (r ? 4 : 5) + (d ? 0 : 1) * 32, transitionColor, 3);
}

bool CloudTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return true;
}

bool CloudTile::interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::shovel)
    {
      if (player.payStamina(5))
      {
        level.setTile(xt, yt, Tile::infiniteFall->id, 0);
        int count = random.nextInt(2) + 1;
        for (int i = 0; i < count; i++)
        {
          level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::cloud), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
        }
        return true;
      }
    }
  }
  return false;
}
