#include "dirttile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

DirtTile::DirtTile(int id) : Tile(id) {}

int DirtTile::getMapColor(Level &level, int x, int y)
{
  return Color::get(level.dirtColor);
}

void DirtTile::render(Screen &screen, Level &level, int x, int y)
{
  int col = Color::get(level.dirtColor, level.dirtColor, level.dirtColor - 111, level.dirtColor - 111);
  screen.renderTile(x * 16 + 0, y * 16 + 0, 0, col, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 0, 1, col, 0);
  screen.renderTile(x * 16 + 0, y * 16 + 8, 2, col, 0);
  screen.renderTile(x * 16 + 8, y * 16 + 8, 3, col, 0);
}

bool DirtTile::interact(Level &level, int xt, int yt, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::shovel)
    {
      if (player.payStamina(4 - tool->level))
      {
        level.setTile(xt, yt, Tile::hole->id, 0);
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::dirt), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
        Sound::monsterHurt.play();
        return true;
      }
    }
    if (tool->type == &ToolType::hoe)
    {
      if (player.payStamina(4 - tool->level))
      {
        level.setTile(xt, yt, Tile::farmland->id, 0);
        Sound::monsterHurt.play();
        return true;
      }
    }
  }
  return false;
}
