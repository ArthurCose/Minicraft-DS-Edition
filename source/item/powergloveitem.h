#pragma once

#include "item.h"

class PowerGloveItem : public Item
{
public:
  int getColor() override;
  int getSprite() override;
  void renderIcon(Screen &screen, int x, int y) override;
  void renderInventory(Screen &screen, int x, int y) override;
  std::string getName() override;
  bool interact(Player &player, Entity &entity, int attackDir) override;
};
