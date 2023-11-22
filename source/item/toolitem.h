#pragma once

#include "item.h"
#include "tooltype.h"
#include "../shim/random.h"

class ToolItem : public Item
{
private:
  Random random;

public:
  static const int MAX_LEVEL = 5;
  ToolType* type;
  int level = 0;

  ToolItem(ToolType* type, int level);

  std::string getName() const override;
  int getColor() const override;
  int getSprite() const override;
  void renderIcon(Screen& screen, int x, int y) override;
  void renderInventory(Screen& screen, int x, int y) override;
  bool canAttack() override;
  int getAttackDamageBonus(Entity& e);
  bool matches(const Item& item) override;
  std::shared_ptr<Item> clone() override;
};