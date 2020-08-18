#pragma once

#include "item.h"
#include "../entity/furniture.h"

class FurnitureItem : public Item
{
public:
  std::shared_ptr<Furniture> furniture;
  bool placed = false;

  FurnitureItem(std::shared_ptr<Furniture> furniture);

  int getColor() override;
  int getSprite() override;
  void renderIcon(Screen &screen, int x, int y) override;
  void renderInventory(Screen &screen, int x, int y) override;
  void onTake(ItemEntity &itemEntity) override;
  bool canAttack() override;
  bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) override;
  bool isDepleted() override;
  std::string getName() override;
  std::shared_ptr<Item> clone() override;
};
