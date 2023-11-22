#include "plantableresource.h"

#include "../../level/level.h"

PlantableResource::PlantableResource(std::string name, int sprite, int color, unsigned char targetTile, std::vector<unsigned char> sourceTiles)
  : Resource(name, sprite, color), targetTile(targetTile)
{
  this->sourceTiles = sourceTiles;
}

bool PlantableResource::interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir) const
{
  for (auto sourceTile : sourceTiles) {
    if (sourceTile == tile.id) {
      level.setTile(xt, yt, targetTile, 0);
      return true;
    }
  }
  return false;
}
