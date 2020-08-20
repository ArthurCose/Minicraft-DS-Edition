#pragma once

#include "../menu.h"
#include "../../entity/player.h"
#include "../../item/inventory.h"

class StorageMenu : public Menu
{
private:
  std::shared_ptr<Player> player;
  std::shared_ptr<Inventory> storageInventory;
  std::string title;
  int selected = 0;
  int oSelected = 0;
  int window = 0;

public:
  StorageMenu(std::string title, std::shared_ptr<Inventory> storageInventory, std::shared_ptr<Player> player);

  void tick(Game &game) override;
  void render(Screen &screen, Screen &bottomScreen) override;
};
