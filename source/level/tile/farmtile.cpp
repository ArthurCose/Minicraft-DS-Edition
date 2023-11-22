#include "farmtile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

FarmTile::FarmTile(int id)
  : Tile(id) {}

int FarmTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(level.dirtColor - 11);
}

void FarmTile::tick(Level& level, int xt, int yt)
{
  int age = level.getData(xt, yt);
  if (age < 5)
    level.setData(xt, yt, age + 1);
}

void FarmTile::render(Screen& screen, Level& level, int x, int y)
{
  int col = Color::get(level.dirtColor - 121, level.dirtColor - 11, level.dirtColor, level.dirtColor + 111);
  screen.renderTile(x * 16 + 0, y * 16 + 0, 2 + 32, col, 1);
  screen.renderTile(x * 16 + 8, y * 16 + 0, 2 + 32, col, 0);
  screen.renderTile(x * 16 + 0, y * 16 + 8, 2 + 32, col, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 8, 2 + 32, col, 1);
}

bool FarmTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::dirt, 0);
        return true;
      }
    }
  }
  return false;
}

void FarmTile::steppedOn(Level& level, int xt, int yt, Entity& entity)
{
  if (random.nextInt(60) != 0)
    return;
  if (level.getData(xt, yt) < 5)
    return;
  level.setTile(xt, yt, Tile::dirt, 0);
}
