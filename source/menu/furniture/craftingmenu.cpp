#include "craftingmenu.h"

#include <algorithm>
#include "../player/ingamemenu.h"

bool compareCraftability(std::shared_ptr<Recipe> &a, std::shared_ptr<Recipe> &b)
{
  return a->canCraft && !b->canCraft;
}

CraftingMenu::CraftingMenu(std::vector<std::shared_ptr<Recipe>> recipes, Player &player)
{
  this->recipes = recipes;

  for (auto &recipe : this->recipes)
  {
    recipe->checkCanCraft(player);
  }

  std::sort(this->recipes.begin(), this->recipes.end(), compareCraftability);

  updateAmounts(player);

  player.itemSelected = false;
}

void CraftingMenu::tick(Game &game)
{
  if (game.justTapped(KEY_B) || game.justTapped(KEY_X) || game.justTapped(KEY_START))
    game.setMenu(std::make_unique<InGameMenu>(game.player, game.levels[game.currentLevel].map));

  int len = recipes.size();

  if (len == 0)
    selected = 0;

  if (game.justTapped(KEY_UP))
  {
    selected--;

    if (selected < 0)
      selected = len - 1;

    updateAmounts(*game.player);
  }

  if (game.justTapped(KEY_DOWN))
  {
    selected++;

    if (selected >= len)
      selected = 0;

    updateAmounts(*game.player);
  }

  if (game.justTapped(KEY_A) && len > 0)
  {
    auto r = recipes[selected];
    Player &player = *game.player;

    if (r->canCraft)
    {
      r->craft(player);

      Sound::craft.play();

      updateAmounts(player);
    }

    for (auto &recipe : this->recipes)
    {
      recipe->checkCanCraft(player);
    }
  }
}

void CraftingMenu::updateAmounts(Player &player)
{

  if (recipes.size() == 0)
  {
    return;
  }

  storedRequiredAmounts.clear();

  auto recipe = recipes[selected];

  storedAmount = player.inventory.count(*recipe->result);

  for (auto &requirement : recipe->requirements)
  {
    storedRequiredAmounts.push_back(player.inventory.count(requirement));
  }
}

void CraftingMenu::render(Screen &screen, Screen &bottomScreen)
{
  screen.renderFrame("Have", 12, 1, 19, 3);
  screen.renderFrame("Cost", 12, 4, 19, 11);
  screen.renderFrame("Crafting", 0, 1, 11, 11);
  renderItemList(screen, 0, 1, 11, 11, recipes, selected);

  if (recipes.size() > 0)
  {
    auto recipe = recipes[selected];
    auto &result = recipe->result;

    int xo = 13 * 8;
    screen.renderTile(xo, 2 * 8, result->getSprite(), result->getColor(), 0);
    screen.renderText(std::to_string(storedAmount), xo + 8, 2 * 8, Color::get(-1, 555, 555, 555));

    for (int i = 0; i < (int)recipe->requirements.size(); i++)
    {
      const Item &item = recipe->requirements[i];
      int yo = (5 + i) * 8;
      screen.renderTile(xo, yo, item.getSprite(), item.getColor(), 0);
      int requiredAmount = 1;

      if (auto resourceItem = dynamic_cast<const ResourceItem *>(&item))
      {
        requiredAmount = resourceItem->count;
      }

      int has = storedRequiredAmounts[i];
      int color = Color::get(-1, 555, 555, 555);

      if (has < requiredAmount)
      {
        color = Color::get(-1, 222, 222, 222);
      }

      if (has > 99)
        has = 99;

      screen.renderText(std::to_string(requiredAmount) + "/" + std::to_string(has), xo + 8, yo, color);
    }
  }
  // renderItemList(screen, 12, 4, 19, 11, recipes.get(selected).costs, -1);
}