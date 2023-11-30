#include "dirttile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"
#include "../../sound/sound.h"

DirtTile::DirtTile(int id) : Tile(id) {}

int DirtTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(level.dirtColor);
}

void DirtTile::render(Screen& screen, Level& level, int x, int y)
{
  auto colors = Color::getArray<8>({ level.dirtColor, level.dirtColor, level.dirtColor - 111, level.dirtColor - 111 });

  screen.renderTile(x * 16, y * 16, 0, colors, 0);
}

bool DirtTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::hole, 0);
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::dirt), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
        Sound::monsterHurt.play();
        return true;
      }
    }
    if (tool->type == &ToolType::hoe) {
      if (player.payStamina(4 - tool->level)) {
        level.setTile(xt, yt, Tile::farmland, 0);
        Sound::monsterHurt.play();
        return true;
      }
    }
  }
  return false;
}
