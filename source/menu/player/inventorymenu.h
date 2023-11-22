#pragma once

#include "../menu.h"
#include "../../entity/player.h"

class InventoryMenu : public Menu
{
private:
  const std::shared_ptr<Player> player;
  int selected = 0;
  int blinkTimer = 0;
  int moving = false;

public:
  InventoryMenu(std::shared_ptr<Player> player);

  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;
};