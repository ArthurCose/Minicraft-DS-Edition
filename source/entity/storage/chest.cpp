#include "chest.h"

#include "../../menu/furniture/storagemenu.h"

Chest::Chest()
  : Furniture("Chest")
{
  inventory = std::make_shared<Inventory>();
  col = Color::get(-1, 110, 331, 552);
  sprite = 1;
}

bool Chest::use(Game& game, Level& level, Player& player, int attackDir)
{
  game.enterMenu(std::make_unique<StorageMenu>(std::string("Chest"), inventory, game.player));
  return true;
}

std::shared_ptr<Furniture> Chest::clone()
{
  return std::make_shared<Chest>();
}
