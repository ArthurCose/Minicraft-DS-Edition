#include "cactustile.h"

#include "../../gfx/color.h"
#include "../../item/resourceitem.h"
#include "../../entity/itementity.h"
#include "../../entity/particle/smashparticle.h"
#include "../../entity/particle/textparticle.h"

CactusTile::CactusTile(int id)
  : Tile(id)
{
  connectsToSand = true;
}

int CactusTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(40);
}

void CactusTile::render(Screen& screen, Level& level, int x, int y)
{
  int col = Color::get(20, 40, 50, level.sandColor);
  screen.renderTile(x * 16 + 0, y * 16 + 0, 8 + 2 * 32, col, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 0, 9 + 2 * 32, col, 0);
  screen.renderTile(x * 16 + 0, y * 16 + 8, 8 + 3 * 32, col, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 8, 9 + 3 * 32, col, 0);
}

bool CactusTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return false;
}

void CactusTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  int damage = level.getData(x, y) + dmg;
  level.add(std::make_shared<SmashParticle>(x * 16 + 8, y * 16 + 8));
  level.add(std::make_shared<TextParticle>(std::to_string(dmg), x * 16 + 8, y * 16 + 8, Color::get(-1, 500, 500, 500)));
  if (damage >= 10) {
    int count = random.nextInt(2) + 1;
    for (int i = 0; i < count; i++) {
      level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::cactusFlower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
    }
    level.setTile(x, y, Tile::sand, 0);
  } else {
    level.setData(x, y, damage);
  }
}

void CactusTile::bumpedInto(Level& level, int x, int y, Entity& entity)
{
  entity.hurt(level, *this, x, y, 1);
}

void CactusTile::tick(Level& level, int xt, int yt)
{
  int damage = level.getData(xt, yt);
  if (damage > 0)
    level.setData(xt, yt, damage - 1);
}
