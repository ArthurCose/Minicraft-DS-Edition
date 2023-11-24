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

void Chest::serializeData(std::ostream& s)
{
  Furniture::serializeData(s);
  inventory->serialize(s, "inventory");
}

void Chest::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name)
{
  if (name == "inventory") {
    inventory->deserialize(s, tag);
  } else {
    Furniture::deserializeDataProperty(s, tag, name);
  }
}
