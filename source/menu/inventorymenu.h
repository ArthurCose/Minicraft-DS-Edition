#pragma once

#include "../entity/player.h"
#include "menu.h"

class InventoryMenu : public Menu
{
private:
  const std::shared_ptr<Player> player;
  int selected = 0;

public:
  InventoryMenu(std::shared_ptr<Player> player);

  void tick(Game &game) override;
  void render(Screen &screen) override;
};