#include "cloudtile.h"

#include "../../item/toolitem.h"
#include "../level.h"
#include "../../entity/entity.h"
#include "../../gfx/screen.h"

CloudTile::CloudTile(int id)
  : Tile(id) {}

int CloudTile::getMapColor(Level& level, int x, int y)
{
  return Color::get(444);
}

void CloudTile::render(Screen& screen, Level& level, int x, int y)
{
  int transitionColor = Color::get(333, 444, 555, 20);

  bool u = level.getTile(x, y - 1) == Tile::infiniteFall;
  bool d = level.getTile(x, y + 1) == Tile::infiniteFall;
  bool l = level.getTile(x - 1, y) == Tile::infiniteFall;
  bool r = level.getTile(x + 1, y) == Tile::infiniteFall;

  bool ul = level.getTile(x - 1, y - 1) == Tile::infiniteFall;
  bool dl = level.getTile(x - 1, y + 1) == Tile::infiniteFall;
  bool ur = level.getTile(x + 1, y - 1) == Tile::infiniteFall;
  bool dr = level.getTile(x + 1, y + 1) == Tile::infiniteFall;

  int offset = (r << 3) | (l << 2) | (d << 1) | u;

  std::array<uint8_t, 8> colors = {
    Color::get(333),
    Color::get(444),
    Color::get(555),
    Color::get(20),
  };

  screen.renderTile(x * 16, y * 16, 4 * 16 + offset, colors, 0);

  if (!u && !l && ul) {
    screen.renderIcon(x * 16 + 0, y * 16 + 0, 7 + 0 * 32, transitionColor, 3);
  }

  if (!u && !r && ur) {
    screen.renderIcon(x * 16 + 8, y * 16 + 0, 8 + 0 * 32, transitionColor, 3);
  }

  if (!d && !l && dl) {
    screen.renderIcon(x * 16 + 0, y * 16 + 8, 7 + 1 * 32, transitionColor, 3);
  }

  if (!d && !r && dr) {
    screen.renderIcon(x * 16 + 8, y * 16 + 8, 8 + 1 * 32, transitionColor, 3);
  }
}

bool CloudTile::mayPass(Level& level, int x, int y, Entity& e)
{
  return true;
}

bool CloudTile::interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
{
  if (auto tool = dynamic_cast<ToolItem*>(&item)) {
    if (tool->type == &ToolType::shovel) {
      if (player.payStamina(5)) {
        level.setTile(xt, yt, Tile::infiniteFall, 0);
        int count = random.nextInt(2) + 1;
        for (int i = 0; i < count; i++) {
          level.add(std::make_shared<ItemEntity>(std::make_shared<ResourceItem>(Resource::cloud), xt * 16 + random.nextInt(10) + 3, yt * 16 + random.nextInt(10) + 3));
        }
        return true;
      }
    }
  }
  return false;
}
