#pragma once

#include "../menu.h"
#include "../../entity/player.h"

class InGameMenu : public Menu
{
public:
  InGameMenu(std::shared_ptr<Player> player, std::shared_ptr<std::vector<unsigned char>> map);

  void tick(Game &game) override;
  void render(Screen &screen, Screen &bottomScreen) override;

private:
  std::shared_ptr<Player> player;
  std::shared_ptr<std::vector<unsigned char>> map;

  void renderHud(Screen &screen);
  void renderMap(Screen &bottomScreen);
};
