#include "treetile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"
#include "../../gfx/screen.h"

TreeTile::TreeTile(int id) : Tile(id)
{
  connectsToGrass = true;
}

int TreeTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(30);
}

void TreeTile::render(Screen &screen, Level &level, int x, int y)
{
  int col = Color::get(10, 30, 151, level.grassColor);
  int barkCol1 = Color::get(10, 30, 430, level.grassColor);
  int barkCol2 = Color::get(10, 30, 320, level.grassColor);

  bool u = level.getTile(x, y - 1) == this;
  bool l = level.getTile(x - 1, y) == this;
  bool r = level.getTile(x + 1, y) == this;
  bool d = level.getTile(x, y + 1) == this;
  bool ul = level.getTile(x - 1, y - 1) == this;
  bool ur = level.getTile(x + 1, y - 1) == this;
  bool dl = level.getTile(x - 1, y + 1) == this;
  bool dr = level.getTile(x + 1, y + 1) == this;

  if (u && ul && l)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 0, 10 + 1 * 32, col, 0);
  }
  else
  {
    screen.renderTile(x * 16 + 0, y * 16 + 0, 9 + 0 * 32, col, 0);
  }
  if (u && ur && r)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 0, 10 + 2 * 32, barkCol2, 0);
  }
  else
  {
    screen.renderTile(x * 16 + 8, y * 16 + 0, 10 + 0 * 32, col, 0);
  }
  if (d && dl && l)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 8, 10 + 2 * 32, barkCol2, 0);
  }
  else
  {
    screen.renderTile(x * 16 + 0, y * 16 + 8, 9 + 1 * 32, barkCol1, 0);
  }
  if (d && dr && r)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 8, 10 + 1 * 32, col, 0);
  }
  else
  {
    screen.renderTile(x * 16 + 8, y * 16 + 8, 10 + 3 * 32, barkCol2, 0);
  }
}

void TreeTile::tick(Level &level, int xt, int yt)
{
  int damage = level.getData(xt, yt);
  if (damage > 0)
    level.setData(xt, yt, damage - 1);
}

bool TreeTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return false;
}

void TreeTile::hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir)
{
  hurt(level, x, y, dmg);
}

bool TreeTile::interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::axe)
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

void TreeTile::hurt(Level &level, int x, int y, int dmg)
{
  {
    int count = random.nextInt(10) == 0 ? 1 : 0;
    for (int i = 0; i < count; i++)
    {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::apple), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
  }
  int damage = level.getData(x, y) + dmg;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (damage >= 20)
  {
    int count = random.nextInt(2) + 1;
    for (int i = 0; i < count; i++)
    {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::wood), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    count = random.nextInt(random.nextInt(4) + 1);
    for (int i = 0; i < count; i++)
    {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::acorn), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    level.setTile(x, y, Tile::grass->id, 0);
  }
  else
  {
    level.setData(x, y, damage);
  }
}
