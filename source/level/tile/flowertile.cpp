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

void FlowerTile::render(Screen &screen, Level &level, int x, int y)
{
  GrassTile::render(screen, level, x, y);

  int data = level.getData(x, y);
  int shape = (data / 16) % 2;
  int flowerCol = Color::get(10, level.grassColor, 555, 440);

  if (shape == 0)
    screen.renderTile(x * 16 + 0, y * 16 + 0, 1 + 1 * 32, flowerCol, 0);
  if (shape == 1)
    screen.renderTile(x * 16 + 8, y * 16 + 0, 1 + 1 * 32, flowerCol, 0);
  if (shape == 1)
    screen.renderTile(x * 16 + 0, y * 16 + 8, 1 + 1 * 32, flowerCol, 0);
  if (shape == 0)
    screen.renderTile(x * 16 + 8, y * 16 + 8, 1 + 1 * 32, flowerCol, 0);
}

bool FlowerTile::interact(Level &level, int x, int y, Player &player, Item &item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem *>(&item))
  {
    if (tool->type == &ToolType::shovel)
    {
      if (player.payStamina(4 - tool->level))
      {
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
        level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
        level.setTile(x, y, Tile::grass->id, 0);
        return true;
      }
    }
  }
  return false;
}

void FlowerTile::hurt(Level &level, int x, int y, Mob &source, int dmg, int attackDir)
{
  int count = random.nextInt(2) + 1;
  for (int i = 0; i < count; i++)
  {
    level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::flower), x * 16 + random.nextInt(10) + 3, y * 16 + random.nextInt(10) + 3));
  }
  level.setTile(x, y, Tile::grass->id, 0);
}
