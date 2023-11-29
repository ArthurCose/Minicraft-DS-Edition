#include "flowertile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

FlowerTile::FlowerTile(int id)
  : GrassTile(id)
{
  tiles[id] = this;
  connectsToGrass = true;
}

int FlowerTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(555);
}

void FlowerTile::render(Screen& screen, Level& level, int x, int y)
{
  int data = level.getData(x, y);
  int shape = (data / 16) % 2;

  std::array<uint8_t, 8> colors = {
    Color::get(10),
    Color::get(level.grassColor),
    Color::get(level.grassColor + 111),
    Color::get(555),
    Color::get(440),
  };

  screen.renderTile(x * 16, y * 16, 6 * 16 + shape, colors, 0);
}

bool FlowerTile::interact(Level& level, int x, int y, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(4 - tool->level)) {
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
        level.setTile(x, y, Tile::grass, 0);
        return true;
      }
    }
  }
  return false;
}

void FlowerTile::hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir)
{
  int count = random.nextInt(2) + 1;
  for (int i = 0; i < count; i++) {
    level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
  }
  level.setTile(x, y, Tile::grass, 0);
}
