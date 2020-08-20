#pragma once

#include <vector>
#include "../menu.h"
#include "../../item/crafting/recipe.h"

class CraftingMenu : public Menu
{
private:
  int selected = 0;
  int storedAmount = 0;
  std::vector<int> storedRequiredAmounts;
  std::vector<std::shared_ptr<Recipe>> recipes;

  void updateAmounts(Player &player);

public:
  CraftingMenu(std::vector<std::shared_ptr<Recipe>> recipes, Player &player);

  void tick(Game &game) override;
  void render(Screen &screen, Screen &bottomScreen) override;
};
