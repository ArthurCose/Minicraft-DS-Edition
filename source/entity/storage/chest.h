#pragma once

#include "../furniture.h"
#include "../../item/inventory.h"

class Chest : public Furniture
{
private:
  std::shared_ptr<Inventory> inventory;

public:
  Chest();

  bool use(Game& game, Level& level, Player& player, int attackDir) override;
  std::shared_ptr<Furniture> clone() override;
};
