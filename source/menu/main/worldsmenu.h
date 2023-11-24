#pragma once

#include "../menu.h"

class WorldsMenu : public Menu
{
public:
  WorldsMenu();

  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;

private:
  int selected = 0;
  std::vector<std::string> worldNames;
};
