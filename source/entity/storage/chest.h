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
  std::string_view serializedClassName() override { return "Chest"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};
