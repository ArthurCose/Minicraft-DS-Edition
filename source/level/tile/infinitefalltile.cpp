#include "infinitefalltile.h"

#include "../../entity/hostile/airwizard.h"
#include "../../gfx/color.h"
#include "../../level/level.h"

InfiniteFallTile::InfiniteFallTile(int id) : Tile(id) {}

int InfiniteFallTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(20);
}

bool InfiniteFallTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return dynamic_cast<AirWizard*>(&e);
}
