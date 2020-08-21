#include "hardrocktile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"
#include "../../gfx/screen.h"

HardRockTile::HardRockTile(int id)
    : Tile(id) {}

int HardRockTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(334);
}

void HardRockTile::tick(Level &level, int xt, int yt)
{
  int damage = level.getData(xt, yt);
  if (damage > 0)
    level.setData(xt, yt, damage - 1);
}

void HardRockTile::render(Screen &screen, Level &level, int x, int y)
{
  int col = Color::get(334, 334, 223, 223);
  int transitionColor = Color::get(001, 334, 445, level.dirtColor);

  bool u = level.getTile(x, y - 1) != this->id;
  bool d = level.getTile(x, y + 1) != this->id;
  bool l = level.getTile(x - 1, y) != this->id;
  bool r = level.getTile(x + 1, y) != this->id;

  bool ul = level.getTile(x - 1, y - 1) != this->id;
  bool dl = level.getTile(x - 1, y + 1) != this->id;
  bool ur = level.getTile(x + 1, y - 1) != this->id;
  bool dr = level.getTile(x + 1, y + 1) != this->id;

  if (!u && !l)
  {
    if (!ul)
      screen.renderTile(x * 16 + 0, y * 16 + 0, 0, col, 0);
    else
      screen.renderTile(x * 16 + 0, y * 16 + 0, 7 + 0 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (l ? 6 : 5) + (u ? 2 : 1) * 32, transitionColor, 3);

  if (!u && !r)
  {
    if (!ur)
      screen.renderTile(x * 16 + 8, y * 16 + 0, 1, col, 0);
    else
      screen.renderTile(x * 16 + 8, y * 16 + 0, 8 + 0 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (r ? 4 : 5) + (u ? 2 : 1) * 32, transitionColor, 3);

  if (!d && !l)
  {
    if (!dl)
      screen.renderTile(x * 16 + 0, y * 16 + 8, 2, col, 0);
    else
      screen.renderTile(x * 16 + 0, y * 16 + 8, 7 + 1 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (l ? 6 : 5) + (d ? 0 : 1) * 32, transitionColor, 3);
  if (!d && !r)
  {
    if (!dr)
      screen.renderTile(x * 16 + 8, y * 16 + 8, 3, col, 0);
    else
      screen.renderTile(x * 16 + 8, y * 16 + 8, 8 + 1 * 32, transitionColor, 3);
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (r ? 4 : 5) + (d ? 0 : 1) * 32, transitionColor, 3);
}

bool HardRockTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return false;
}

void HardRockTile::hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir)
{
  hurt(level, x, y, 0);
}

bool HardRockTile::interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::pickaxe && tool->level == 4)
    {
      if (player.payStamina(4 - tool->level))
      {
        hurt(level, xt, yt, random.nextInt(10) + (tool->level) * 5 + 10);
        return true;
      }
    }
  }
  return false;
}

void HardRockTile::hurt(Level &level, int x, int y, int dmg)
{
  int damage = level.getData(x, y) + dmg;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (damage >= 200)
  {
    int count = random.nextInt(4) + 1;
    for (int i = 0; i < count; i++)
    {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::stone), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    count = random.nextInt(2);
    for (int i = 0; i < count; i++)
    {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::coal), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    level.setTile(x, y, Tile::dirt, 0);
  }
  else
  {
    level.setData(x, y, damage);
  }
}
