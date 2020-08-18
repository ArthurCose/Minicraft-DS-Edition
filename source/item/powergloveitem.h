#pragma once

#include "item.h"

class PowerGloveItem : public Item
{
public:
  std::string getName() const override;
  int getColor() const override;
  int getSprite() const override;
  void renderIcon(Screen &screen, int x, int y) override;
  void renderInventory(Screen &screen, int x, int y) override;
  bool interact(Player &player, Entity &entity, int attackDir) override;
  std::shared_ptr<Item> clone() override;
};
