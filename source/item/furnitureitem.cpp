#include "furnitureitem.h"

FurnitureItem::FurnitureItem(std::shared_ptr<Furniture> furniture)
  : furniture(furniture) {}

std::string_view FurnitureItem::getName() const
{
  return furniture->name;
}

int FurnitureItem::getColor() const
{
  return furniture->col;
}

int FurnitureItem::getSprite() const
{
  return furniture->sprite + 10 * 32;
}

void FurnitureItem::renderIcon(Screen& screen, int x, int y)
{
  screen.renderIcon(x, y, getSprite(), getColor(), 0);
}

void FurnitureItem::renderInventory(Screen& screen, int x, int y)
{
  screen.renderIcon(x, y, getSprite(), getColor(), 0);
  screen.renderText(furniture->name, x + 8, y, Color::get(-1, 555, 555, 555));
}

bool FurnitureItem::canAttack()
{
  return false;
}

bool FurnitureItem::interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir)
{
  if (tile.mayPass(level, xt, yt, *furniture)) {
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

std::shared_ptr<Item> FurnitureItem::clone()
{
  return std::make_shared<FurnitureItem>(furniture->clone());
}

void FurnitureItem::serializeData(std::ostream& s)
{
  Item::serializeData(s);
  nbt::begin_named_compound(s, "furniture");
  furniture->serialize(s);
  nbt::close_compound(s);
}

void FurnitureItem::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name)
{
  if (name == "furniture") {
    std::shared_ptr entity = Entity::deserialize(s, tag);
    furniture = std::dynamic_pointer_cast<Furniture>(entity);
  } else {
    Item::deserializeDataProperty(s, tag, name);
  }
}
