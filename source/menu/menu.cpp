#include "menu.h"

void Menu::setParent(std::unique_ptr<Menu> menu)
{
  parent = std::move(menu);
}

void Menu::close(Game& game)
{
  game.setMenu(std::move(parent));
}
