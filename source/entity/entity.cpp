#include "entity.h"

#include <ctime>
#include <algorithm>
#include <vector>

#include "../level/level.h"

Random Entity::random(time(NULL));

void Entity::remove()
{
  removed = true;
}

bool Entity::intersects(int x0, int y0, int x1, int y1)
{
  return !(x + xr < x0 || y + yr < y0 || x - xr > x1 || y - yr > y1);
}

bool Entity::move(Level& level, int xa, int ya)
{
  if (xa != 0 || ya != 0) {
    bool stopped = true;
    if (xa != 0 && move2(level, xa, 0))
      stopped = false;
    if (ya != 0 && move2(level, 0, ya))
      stopped = false;
    if (!stopped) {
      int xt = x >> 4;
      int yt = y >> 4;
      Tile::tiles[level.getTile(xt, yt)]->steppedOn(level, xt, yt, *this);
    }
    return !stopped;
  }
  return true;
}

bool Entity::move2(Level& level, int xa, int ya)
{
  sassert(xa != 0 || ya != 0, "Entity.move2() should be called if you're moving");
  sassert((xa != 0) != (ya != 0), "Entity.move2() can only move along one axis at a time");

  int xto0 = ((x)-xr) >> 4;
  int yto0 = ((y)-yr) >> 4;
  int xto1 = ((x)+xr) >> 4;
  int yto1 = ((y)+yr) >> 4;

  int xt0 = ((x + xa) - xr) >> 4;
  int yt0 = ((y + ya) - yr) >> 4;
  int xt1 = ((x + xa) + xr) >> 4;
  int yt1 = ((y + ya) + yr) >> 4;
  bool blocked = false;

  for (int yt = yt0; yt <= yt1; yt++)
    for (int xt = xt0; xt <= xt1; xt++) {
      if (xt >= xto0 && xt <= xto1 && yt >= yto0 && yt <= yto1)
        continue;

      Tile* tile = Tile::tiles[level.getTile(xt, yt)];

      tile->bumpedInto(level, xt, yt, *this);

      if (!tile->mayPass(level, xt, yt, *this)) {
        blocked = true;
        return false;
      }
    }
  if (blocked)
    return false;

  auto wasInside = level.getEntities(x - xr, y - yr, x + xr, y + yr);
  auto isInside = level.getEntities(x + xa - xr, y + ya - yr, x + xa + xr, y + ya + yr);

  for (auto e : isInside) {
    if (e.get() == this)
      continue;

    e->touchedBy(level, *this);
  }

  auto wasInsideBegin = wasInside.begin();
  auto wasInsideEnd = wasInside.end();

  for (auto e : isInside) {
    if (e.get() == this)
      continue;

    if (std::find(wasInsideBegin, wasInsideEnd, e) == wasInsideEnd) {
      continue;
    }

    if (e->blocks(*this)) {
      return false;
    }
  }

  x += xa;
  y += ya;
  return true;
}

bool Entity::interact(Player& player, Item& item, int attackDir)
{
  return item.interact(player, *this, attackDir);
}
