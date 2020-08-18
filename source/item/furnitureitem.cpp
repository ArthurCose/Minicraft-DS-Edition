#include "furnitureitem.h"

FurnitureItem::FurnitureItem(std::shared_ptr<Furniture> furniture)
    : furniture(furniture) {}

int FurnitureItem::getColor()
{
  return furniture->col;
}

int FurnitureItem::getSprite()
{
  return furniture->sprite + 10 * 32;
}

void FurnitureItem::renderIcon(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
}

void FurnitureItem::renderInventory(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
  screen.renderText(furniture->name, x + 8, y, Color::get(-1, 555, 555, 555));
}

void FurnitureItem::onTake(ItemEntity &itemEntity)
{
}

bool FurnitureItem::canAttack()
{
  return false;
}

bool FurnitureItem::interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir)
{
  if (tile.mayPass(level, xt, yt, *furniture))
  {
    furniture->x = xt * 16 + 8;
    furniture->y = yt * 16 + 8;
    level.add(furniture);
    placed = true;
    return true;
  }
  return false;
}

bool FurnitureItem::isDepleted()
{
  return placed;
}

std::string FurnitureItem::getName()
{
  return furniture->name;
}

std::shared_ptr<Item> FurnitureItem::clone()
{
  return std::make_shared<FurnitureItem>(furniture->clone());
}
