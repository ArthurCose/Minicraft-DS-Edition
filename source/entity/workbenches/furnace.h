#pragma once

#include "../furniture.h"

class Furnace : public Furniture
{
public:
  Furnace();

  bool use(Game &game, Level &level, Player &player, int attackDir) override;
};
