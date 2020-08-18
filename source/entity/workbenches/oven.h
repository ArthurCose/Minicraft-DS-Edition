#pragma once

#include "../furniture.h"

class Oven : public Furniture
{
public:
  Oven();

  bool use(Game &game, Level &level, Player &player, int attackDir) override;
  std::shared_ptr<Furniture> clone() override;
};
