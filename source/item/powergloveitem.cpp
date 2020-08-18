#include "powergloveitem.h"

#include "../gfx/color.h"
#include "../entity/furniture.h"

std::string PowerGloveItem::getName() const
{
  return "Pow glove";
}

int PowerGloveItem::getColor() const
{
  return Color::get(-1, 100, 320, 430);
}

int PowerGloveItem::getSprite() const
{
  return 7 + 4 * 32;
}

void PowerGloveItem::renderIcon(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
}

void PowerGloveItem::renderInventory(Screen &screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
  screen.renderText(getName(), x + 8, y, Color::get(-1, 555, 555, 555));
}

bool PowerGloveItem::interact(Player &player, Entity &entity, int attackDir)
{
  if (auto furniture = dynamic_cast<Furniture *>(&entity))
  {
    furniture->take(player);
    return true;
  }
  return false;
}

std::shared_ptr<Item> PowerGloveItem::clone()
{
  return std::make_shared<PowerGloveItem>();
}
