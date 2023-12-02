#pragma once

#include "item.h"
#include "../entity/furniture.h"

class FurnitureItem : public Item
{
public:
  std::shared_ptr<Furniture> furniture;
  bool placed = false;

  FurnitureItem(std::shared_ptr<Furniture> furniture);

  std::string_view getName() const override;
  int getColor() const override;
  int getSprite() const override;
  void renderIcon(Screen& screen, int x, int y) override;
  void renderInventory(Screen& screen, int x, int y) override;
  bool canAttack() override;
  bool interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir) override;
  bool isDepleted() override;
  std::shared_ptr<Item> clone() override;
  std::string_view serializedClassName() override { return "FurnitureItem"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};
