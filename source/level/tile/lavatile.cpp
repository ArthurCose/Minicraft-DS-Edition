#include "lavatile.h"

#include "../level.h"
#include "../../gfx/screen.h"

static Random wRandom;

LavaTile::LavaTile(int id)
  : Tile(id)
{
  connectsToSand = true;
  connectsToLava = true;
}

int LavaTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(500);
}

void LavaTile::render(Screen& screen, Level& level, int x, int y)
{
  wRandom.setSeed((tickCount + (x / 2 - y) * 4311) / 10 * 54687121l + x * 3271612l + y * 3412987161l);
  int col = Color::get(500, 500, 520, 550);
  int transitionColor1 = Color::get(3, 500, level.dirtColor - 111, level.dirtColor);
  int transitionColor2 = Color::get(3, 500, level.sandColor - 110, level.sandColor);

  auto u = Tile::tiles[level.getTile(x, y - 1)];
  auto d = Tile::tiles[level.getTile(x, y + 1)];
  auto l = Tile::tiles[level.getTile(x - 1, y)];
  auto r = Tile::tiles[level.getTile(x + 1, y)];

  bool lu = !u->connectsToLava;
  bool ld = !d->connectsToLava;
  bool ll = !l->connectsToLava;
  bool lr = !r->connectsToLava;

  bool su = lu && u->connectsToSand;
  bool sd = ld && d->connectsToSand;
  bool sl = ll && l->connectsToSand;
  bool sr = lr && r->connectsToSand;

  int randomInt = wRandom.nextBits(16);

  auto renderLiquid = [&screen, &randomInt, col](int x, int y) {
    int tile = randomInt & 0b11;
    randomInt >>= 2;
    int bits = randomInt & 0b11;
    randomInt >>= 2;

    screen.renderTile(x, y, tile, col, bits);
  };

  if (!lu && !ll) {
    renderLiquid(x * 16 + 0, y * 16 + 0);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 0, (ll ? 14 : 15) + (lu ? 0 : 1) * 32, (su || sl) ? transitionColor2 : transitionColor1, 0);

  if (!lu && !lr) {
    renderLiquid(x * 16 + 8, y * 16 + 0);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 0, (lr ? 16 : 15) + (lu ? 0 : 1) * 32, (su || sr) ? transitionColor2 : transitionColor1, 0);

  if (!ld && !ll) {
    renderLiquid(x * 16 + 0, y * 16 + 8);
  } else
    screen.renderTile(x * 16 + 0, y * 16 + 8, (ll ? 14 : 15) + (ld ? 2 : 1) * 32, (sd || sl) ? transitionColor2 : transitionColor1, 0);
  if (!ld && !lr) {
    renderLiquid(x * 16 + 8, y * 16 + 8);
  } else
    screen.renderTile(x * 16 + 8, y * 16 + 8, (lr ? 16 : 15) + (ld ? 2 : 1) * 32, (sd || sr) ? transitionColor2 : transitionColor1, 0);
}

bool LavaTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return e.canSwim();
}

void LavaTile::tick(Level& level, int xt, int yt)
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

int LavaTile::getLightRadius(Level& level, int x, int y)
{
  return 6;
}
