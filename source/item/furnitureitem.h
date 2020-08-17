#pragma once

#include "item.h"
#include "../entity/furniture.h"

class FurnitureItem : public Item
{
public:
  Furniture furniture;
  bool placed = false;

  FurnitureItem(Furniture furniture)
      : furniture(furniture) {}

  int getColor()
  {
    return furniture.col;
  }

  int getSprite()
  {
    return furniture.sprite + 10 * 32;
  }

  void renderIcon(Screen &screen, int x, int y)
  {
    screen.renderTile(x, y, getSprite(), getColor(), 0);
  }

  void renderInventory(Screen &screen, int x, int y)
  {
    screen.renderTile(x, y, getSprite(), getColor(), 0);
    screen.renderText(furniture.name, x + 8, y, Color::get(-1, 555, 555, 555));
  }

  void onTake(ItemEntity &itemEntity)
  {
  }

  bool canAttack()
  {
    return false;
  }

  bool interactOn(Tile tile, Level level, int xt, int yt, Player player, int attackDir)
  {
    if (tile.mayPass(level, xt, yt, furniture))
    {
      furniture.x = xt * 16 + 8;
      furniture.y = yt * 16 + 8;
      level.add(std::make_shared<Furniture>(furniture));
      placed = true;
      return true;
    }
    return false;
  }

  bool isDepleted()
  {
    return placed;
  }

  std::string getName()
  {
    return furniture.name;
  }
};