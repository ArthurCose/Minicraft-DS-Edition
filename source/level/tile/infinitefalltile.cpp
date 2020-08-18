#include "infinitefalltile.h"

#include "../../entity/hostile/airwizard.h"

InfiniteFallTile::InfiniteFallTile(int id) : Tile(id) {}

bool InfiniteFallTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return dynamic_cast<AirWizard *>(&e);
}
