#include "watertile.h"

#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

WaterTile::WaterTile(int id) : Tile(id)
{
  connectsToSand = true;
  connectsToWater = true;
}

void WaterTile::render(Screen &screen, Level &level, int x, int y)
{
  wRandom.setSeed((tickCount + (x / 2 - y) * 4311) / 10 * 54687121l + x * 3271612l + y * 3412987161l);
  int col = Color::get(005, 005, 115, 115);
  int transitionColor1 = Color::get(3, 005, level.dirtColor - 111, level.dirtColor);
  int transitionColor2 = Color::get(3, 005, level.sandColor - 110, level.sandColor);

  auto u = level.getTile(x, y - 1);
  auto d = level.getTile(x, y + 1);
  auto l = level.getTile(x - 1, y);
  auto r = level.getTile(x + 1, y);

  bool wu = !u->connectsToWater;
  bool wd = !d->connectsToWater;
  bool wl = !l->connectsToWater;
  bool wr = !r->connectsToWater;

  bool su = wu && u->connectsToSand;
  bool sd = wd && d->connectsToSand;
  bool sl = wl && l->connectsToSand;
  bool sr = wr && r->connectsToSand;

  if (!wu && !wl)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 0, wRandom.nextInt(4), col, wRandom.nextInt(4));
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (wl ? 14 : 15) + (wu ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);

  if (!wu && !wr)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 0, wRandom.nextInt(4), col, wRandom.nextInt(4));
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (wr ? 16 : 15) + (wu ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);

  if (!wd && !wl)
  {
    screen.renderTile(x * 16 + 0, y * 16 + 8, wRandom.nextInt(4), col, wRandom.nextInt(4));
  }
  else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (wl ? 14 : 15) + (wd ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);

  if (!wd && !wr)
  {
    screen.renderTile(x * 16 + 8, y * 16 + 8, wRandom.nextInt(4), col, wRandom.nextInt(4));
  }
  else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (wr ? 16 : 15) + (wd ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
}

bool WaterTile::mayPass(Level &level, int x, int y, Entity &e)
{
  return e.canSwim();
}

void WaterTile::tick(Level &level, int xt, int yt)
{
  int xn = xt;
  int yn = yt;

  if (random.nextBoolean())
    xn += random.nextInt(2) * 2 - 1;
  else
    yn += random.nextInt(2) * 2 - 1;

  if (level.getTile(xn, yn) == Tile::hole)
  {
    level.setTile(xn, yn, this->id, 0);
  }
}
