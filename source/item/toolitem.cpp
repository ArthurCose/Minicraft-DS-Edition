#include "toolitem.h"

#include "../gfx/color.h"

const std::array<std::string_view, ToolItem::MAX_LEVEL> ToolItem::LEVEL_NAMES = { "Wood", "Rock", "Iron", "Gold", "Gem" };
const std::array<int, ToolItem::MAX_LEVEL> ToolItem::LEVEL_COLORS = {
  Color::get(-1, 100, 321, 431),
  Color::get(-1, 100, 321, 111),
  Color::get(-1, 100, 321, 555),
  Color::get(-1, 100, 321, 550),
  Color::get(-1, 100, 321, 055),
};;

ToolItem::ToolItem(ToolType* type, int level)
{
  this->type = type;
  this->level = level;
}

std::string_view ToolItem::getName() const
{
  return type->levelNames[level];
}

int ToolItem::getColor() const
{
  return LEVEL_COLORS[level];
}

int ToolItem::getSprite() const
{
  return type->sprite + 5 * 32;
}

void ToolItem::renderIcon(Screen& screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
}

void ToolItem::renderInventory(Screen& screen, int x, int y)
{
  screen.renderTile(x, y, getSprite(), getColor(), 0);
  screen.renderText(getName(), x + 8, y, Color::get(-1, 555, 555, 555));
}

bool ToolItem::canAttack()
{
  return true;
}

int ToolItem::getAttackDamageBonus(Entity& e)
{
  if (type == &ToolType::axe) {
    return (level + 1) * 2 + random.nextInt(4);
  }
  if (type == &ToolType::sword) {
    return (level + 1) * 3 + random.nextInt(2 + level * level * 2);
  }
  return 1;
}

bool ToolItem::matches(const Item& item)
{
  if (auto other = dynamic_cast<const ToolItem*>(&item)) {
    if (other->type != type)
      return false;
    if (other->level != level)
      return false;
    return true;
  }
  return false;
}

std::shared_ptr<Item> ToolItem::clone()
{
  return std::make_shared<ToolItem>(type, level);
}

void ToolItem::serializeData(std::ostream& s)
{
  Item::serializeData(s);
  nbt::write_named_string(s, "type", type->name);
  nbt::write_named_int(s, "level", level);
}

void ToolItem::deserializeDataProperty(std::istream& s, nbt::Tag tag, std::string_view name)
{
  if (name == "type") {
    auto typeName = nbt::read_tagged_string(s, tag);

    std::array<ToolType*, 5> toolTypes = {
      &ToolType::shovel,
      &ToolType::hoe,
      &ToolType::sword,
      &ToolType::pickaxe,
      &ToolType::axe
    };

    for (ToolType* toolType : toolTypes) {
      if (typeName == toolType->name) {
        type = toolType;
      }
    }
  } else if (name == "level") {
    level = nbt::read_tagged_number<int>(s, tag);
  } else {
    Item::deserializeDataProperty(s, tag, name);
  }
}
