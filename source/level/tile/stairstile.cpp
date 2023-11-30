#include "stairstile.h"

#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

StairsTile::StairsTile(int id, bool leadsUp)
  : Tile(id)
{
  this->leadsUp = leadsUp;
}

int StairsTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(333);
}

void StairsTile::render(Screen& screen, Level& level, int x, int y)
{
  auto colors = Color::getArray<8>({ level.dirtColor, 0, 333, 444, level.sandColor });

  screen.renderTile(x * 16, y * 16, 6 * 16 + 3 + leadsUp, colors, 0);
}
