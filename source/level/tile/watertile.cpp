#include "watertile.h"

#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

static Random wRandom;

WaterTile::WaterTile(int id) : Tile(id)
{
  connectsToSand = true;
  connectsToWater = true;
}

int WaterTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(005);
}

void WaterTile::render(Screen& screen, Level& level, int x, int y)
{
  wRandom.setSeed((tickCount + (x / 2 - y) * 4311) / 10 * 54687121l + x * 3271612l + y * 3412987161l);
  int transitionColor1 = Color::get(3, 005, level.dirtColor - 111, level.dirtColor);
  int transitionColor2 = Color::get(3, 005, level.sandColor - 110, level.sandColor);

  auto u = Tile::tiles[level.getTile(x, y - 1)];
  auto d = Tile::tiles[level.getTile(x, y + 1)];
  auto l = Tile::tiles[level.getTile(x - 1, y)];
  auto r = Tile::tiles[level.getTile(x + 1, y)];

  bool wu = !u->connectsToWater;
  bool wd = !d->connectsToWater;
  bool wl = !l->connectsToWater;
  bool wr = !r->connectsToWater;

  bool su = wu && u->connectsToSand;
  bool sd = wd && d->connectsToSand;
  bool sl = wl && l->connectsToSand;
  bool sr = wr && r->connectsToSand;

  int randomInt = wRandom.nextInt() & 0b11;

  auto colors = Color::getArray<8>({ 005, 005, 115, 115 });

  screen.renderTile(x * 16, y * 16, 0, colors, randomInt);

  if (wu || wl) {
    screen.renderIcon(x * 16 + 0, y * 16 + 0, (wl ? 14 : 15) + (wu ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);
  }

  if (wu || wr) {
    screen.renderIcon(x * 16 + 8, y * 16 + 0, (wr ? 16 : 15) + (wu ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);
  }

  if (wd || wl) {
    screen.renderIcon(x * 16 + 0, y * 16 + 8, (wl ? 14 : 15) + (wd ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);
  }

  if (wd || wr) {
    screen.renderIcon(x * 16 + 8, y * 16 + 8, (wr ? 16 : 15) + (wd ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
  }
}

bool WaterTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return e.canSwim();
}

void WaterTile::tick(Level& level, int xt, int yt)
{
  if (
    level.getTile(xt - 1, yt) != Tile::hole &&
    level.getTile(xt + 1, yt) != Tile::hole &&
    level.getTile(xt, yt - 1) != Tile::hole &&
    level.getTile(xt, yt + 1) != Tile::hole
    ) {
    // check for hole tiles as random number gen is expensive
    return;
  }

  int xn = xt;
  int yn = yt;

  if (random.nextInt(2))
    xn += random.nextInt(2) * 2 - 1;
  else
    yn += random.nextInt(2) * 2 - 1;

  if (level.getTile(xn, yn) == Tile::hole) {
    level.setTile(xn, yn, this->id, 0);
  }
}
