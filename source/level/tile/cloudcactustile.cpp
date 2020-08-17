#include "cloudcactustile.h"

#include "../../gfx/color.h"
#include "../../item/resourceitem.h"
#include "../../item/toolitem.h"
#include "../../entity/airwizard.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"

CloudCactusTile::CloudCactusTile(int id)
    : Tile(id)
{
}

void CloudCactusTile::render(Screen &screen, Level &level, int x, int y)
{
  int color = Color::get(444, 111, 333, 555);
  screen.renderTile(x * 16 + 0, y * 16 + 0, 17 + 1 * 32, color, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 0, 18 + 1 * 32, color, 0);
  screen.renderTile(x * 16 + 0, y * 16 + 8, 17 + 2 * 32, color, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 8, 18 + 2 * 32, color, 0);
}

bool CloudCactusTile::mayPass(Level &level, int x, int y, Entity &e)
{
  if (dynamic_cast<AirWizard *>(&e))
    return true;
  return false;
}

void CloudCactusTile::hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir)
{
  hurt(level, x, y, 0);
}

bool CloudCactusTile::interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::pickaxe)
    {
      if (player.payStamina(6 - tool->level))
      {
        hurt(level, xt, yt, 1);
        return true;
      }
    }
  }
  return false;
}

void CloudCactusTile::hurt(Level &level, int x, int y, int dmg)
{
  int damage = level.getData(x, y) + 1;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (dmg > 0)
  {
    if (damage >= 10)
    {
      level.setTile(x, y, Tile::cloud->id, 0);
    }
    else
    {
      level.setData(x, y, damage);
    }
  }
}

void CloudCactusTile::bumpedInto(Level &level, int x, int y, Entity &entity)
{
  if (dynamic_cast<AirWizard *>(&entity))
    return;
  entity.hurt(level, *this, x, y, 3);
}
