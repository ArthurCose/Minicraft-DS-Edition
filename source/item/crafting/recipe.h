#pragma once

#include <memory>
#include "../../menu/listitem.h"
#include "../item.h"
#include "../resourceitem.h"

class Recipe : public ListItem
{
public:
  const std::unique_ptr<Item> result;
  const std::vector<ResourceItem> requirements;
  bool canCraft = false;

  Recipe(std::unique_ptr<Item> result, std::vector<ResourceItem> requirements);

  Recipe& addRequirement(const Resource* resource, int count);
  void craft(Player& player);
  void checkCanCraft(Player& player);
  void renderInventory(Screen& screen, int x, int y) override;
  void deductCost(Player& player);
};
