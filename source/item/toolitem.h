#pragma once

#include <string_view>
#include "item.h"
#include "tooltype.h"
#include "../shim/random.h"

class ToolItem : public Item
{
private:
  Random random;

public:
  static const int MAX_LEVEL = 5;
  static const std::array<std::string_view, ToolItem::MAX_LEVEL> LEVEL_NAMES;
  static const std::array<int, ToolItem::MAX_LEVEL> LEVEL_COLORS;

  ToolType* type;
  int level = 0;

  ToolItem(ToolType* type, int level);

  std::string_view getName() const override;
  int getColor() const override;
  int getSprite() const override;
  void renderIcon(Screen& screen, int x, int y) override;
  void renderInventory(Screen& screen, int x, int y) override;
  bool canAttack() override;
  int getAttackDamageBonus(Entity& e);
  bool matches(const Item& item) override;
  std::shared_ptr<Item> clone() override;
  std::string_view serializedClassName() override { return "ToolItem"; }
  void serializeData(std::ostream& s) override;
  void deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name) override;
};