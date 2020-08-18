#include "workbench.h"

#include "oven.h"
#include "furnace.h"
#include "anvil.h"
#include "../../item/toolitem.h"
#include "../../item/furnitureitem.h"
#include "../../menu/craftingmenu.h"

static std::vector<std::shared_ptr<Recipe>> workbenchRecipes = {
    // std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Lantern>()), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::slime, 10), ResourceItem(Resource::glass, 4)})),
    std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Oven>()), std::vector<ResourceItem>({ResourceItem(Resource::stone, 15)})),
    std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Furnace>()), std::vector<ResourceItem>({ResourceItem(Resource::stone, 20)})),
    std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Workbench>()), std::vector<ResourceItem>({ResourceItem(Resource::wood, 20)})),
    // std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Chest>()), std::vector<ResourceItem>({ResourceItem(Resource::wood, 20)})),
    std::make_shared<Recipe>(std::make_unique<FurnitureItem>(std::make_shared<Anvil>()), std::vector<ResourceItem>({ResourceItem(Resource::ironIngot, 5)})),

    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::sword, 0), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::axe, 0), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::hoe, 0), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::pickaxe, 0), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::shovel, 0), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5)})),

    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::sword, 1), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::stone, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::axe, 1), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::stone, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::hoe, 1), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::stone, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::pickaxe, 1), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::stone, 5)})),
    std::make_shared<Recipe>(std::make_unique<ToolItem>(&ToolType::shovel, 1), std::vector<ResourceItem>({ResourceItem(Resource::wood, 5), ResourceItem(Resource::stone, 5)})),
};

Workbench::Workbench() : Furniture("Workbench")
{
  col = Color::get(-1, 100, 321, 431);
  sprite = 4;
  xr = 3;
  yr = 2;
}

bool Workbench::use(Game &game, Level &level, Player &player, int attackDir)
{
  game.setMenu(std::make_unique<CraftingMenu>(workbenchRecipes, player));
  return true;
}

std::shared_ptr<Furniture> Workbench::clone()
{
  return std::make_shared<Workbench>();
}
