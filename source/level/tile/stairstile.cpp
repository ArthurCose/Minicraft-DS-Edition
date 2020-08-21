#include "stairstile.h"

#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

StairsTile::StairsTile(int id, bool leadsUp)
    : Tile(id)
{
  this->leadsUp = leadsUp;
}

int StairsTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(333);
}

void StairsTile::render(Screen &screen, Level &level, int x, int y)
{
  int color = Color::get(level.dirtColor, 000, 333, 444);
  int xt = 0;

  if (leadsUp)
    xt = 2;

  screen.renderTile(x * 16 + 0, y * 16 + 0, xt + 2 * 32, color, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 0, xt + 1 + 2 * 32, color, 0);
  screen.renderTile(x * 16 + 0, y * 16 + 8, xt + 3 * 32, color, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 8, xt + 1 + 3 * 32, color, 0);
}
