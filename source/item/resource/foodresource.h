#pragma once

#include <string>
#include "resource.h"

class FoodResource : public Resource
{
private:
  const int heal;
  const int staminaCost;

public:
  FoodResource(std::string name, int sprite, int color, int heal, int staminaCost);

  bool interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir) const override;
};
