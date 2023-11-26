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
  int col = Color::get(005, 005, 115, 115);
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

  int randomInt = wRandom.nextBits(8);

  auto renderLiquid = [&screen, &randomInt, col](int x, int y) {
    int tile = randomInt & 0b11;
    randomInt >>= 2;
    int bits = randomInt & 0b11;
    randomInt >>= 2;

    screen.renderTile(x, y, tile, col, bits);
  };

  if (!wu && !wl) {
    renderLiquid(x * 16 + 0, y * 16 + 0);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (wl ? 14 : 15) + (wu ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);

  if (!wu && !wr) {
    renderLiquid(x * 16 + 8, y * 16 + 0);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (wr ? 16 : 15) + (wu ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);

  if (!wd && !wl) {
    renderLiquid(x * 16 + 0, y * 16 + 8);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (wl ? 14 : 15) + (wd ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);

  if (!wd && !wr) {
    renderLiquid(x * 16 + 8, y * 16 + 8);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (wr ? 16 : 15) + (wd ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
}

bool WaterTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return e.canSwim();
}

void WaterTile::tick(Level& level, int xt, int yt)
{
  int xn = xt;
  int yn = yt;

  if (random.nextBoolean())
    xn += random.nextBits(1) * 2 - 1;
  else
    yn += random.nextBits(1) * 2 - 1;

  if (level.getTile(xn, yn) == Tile::hole) {
    level.setTile(xn, yn, this->id, 0);
  }
}
