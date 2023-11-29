#include "cloudcactustile.h"

#include "../../gfx/color.h"
#include "../../item/resourceitem.h"
#include "../../item/toolitem.h"
#include "../../entity/hostile/airwizard.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"

CloudCactusTile::CloudCactusTile(int id)
  : Tile(id)
{
}

int CloudCactusTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(333);
}

void CloudCactusTile::render(Screen& screen, Level& level, int x, int y)
{
  std::array<uint8_t, 8> colors = {
    Color::get(444),
    Color::get(111),
    Color::get(333),
    Color::get(555),
  };

  screen.renderTile(x * 16, y * 16, 6 * 16 + 5, colors, 0);
}

bool CloudCactusTile::mayPass(Level& level, int x, int y, Entity& e)
{
  if (dynamic_cast<AirWizard*>(&e))
    return true;
  return false;
}

void CloudCactusTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  hurt(level, x, y, 0);
}

bool CloudCactusTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::pickaxe) {
      if (player.payStamina(6 - tool->level)) {
        hurt(level, xt, yt, 1);
        return true;
      }
    }
  }
  return false;
}

void CloudCactusTile::hurt(Level& level, int x, int y, int dmg)
{
  int damage = level.getData(x, y) + 1;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (dmg > 0) {
    if (damage >= 10) {
      level.setTile(x, y, Tile::cloud, 0);
    } else {
      level.setData(x, y, damage);
    }
  }
}

void CloudCactusTile::bumpedInto(Level& level, int x, int y, Entity& entity)
{
  if (dynamic_cast<AirWizard*>(&entity))
    return;
  entity.hurt(level, *this, x, y, 3);
}
