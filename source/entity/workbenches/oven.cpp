#include "oven.h"

#include "../../item/resourceitem.h"
#include "../../item/resource/foodresource.h"
#include "../../menu/furniture/craftingmenu.h"

static std::vector<std::shared_ptr<Recipe>> ovenRecipes = {
    std::make_shared<Recipe>(std::make_unique<ResourceItem>(FoodResource::bread), std::vector<ResourceItem>({ResourceItem(Resource::wheat, 4)})),
};

Oven::Oven()
    : Furniture("Oven")
{
  col = Color::get(-1, 000, 332, 442);
  sprite = 2;
  xr = 3;
  yr = 2;
}

bool Oven::use(Game &game, Level &level, Player &player, int attackDir)
{
  game.enterMenu(std::make_unique<CraftingMenu>(ovenRecipes, player));
  return true;
}

std::shared_ptr<Furniture> Oven::clone()
{
  return std::make_shared<Oven>();
}
