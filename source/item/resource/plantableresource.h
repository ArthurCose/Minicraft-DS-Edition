#pragma once

#include "resource.h"
#include "../../level/tile/tile.h"

#include <vector>

class PlantableResource : public Resource
{
private:
  std::vector<unsigned char> sourceTiles;
  char targetTile;

public:
  PlantableResource(std::string name, int sprite, int color, unsigned char targetTile, std::vector<unsigned char> sourceTiles);

  bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) const override;
};
