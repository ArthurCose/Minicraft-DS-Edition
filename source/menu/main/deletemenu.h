#pragma once

#include "../menu.h"

class DeleteMenu : public Menu
{
public:
  DeleteMenu(std::string name, std::function<void(bool)> callback);

  void tick(Game& game) override;
  void render(Screen& screen, Screen& bottomScreen) override;

private:
  bool yes = true;
  std::string title;
  std::function<void(bool)> callback;
};
