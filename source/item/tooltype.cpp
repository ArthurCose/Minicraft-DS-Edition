#include "tooltype.h"

#include <nds/arm9/sassert.h>
#include "toolitem.h"

ToolType::ToolType(std::string name, int sprite) :
  name(name),
  sprite(sprite)
{
  sassert(levelNames.size() == ToolItem::LEVEL_NAMES.size(), "levelNames.size() == ToolItem::LEVEL_NAMES.size()");

  for (size_t i = 0; i < levelNames.size(); i++) {
    levelNames[i] = std::string(ToolItem::LEVEL_NAMES[i]) + " " + name;
  }
}

ToolType ToolType::shovel = ToolType("Shvl", 0);
ToolType ToolType::hoe = ToolType("Hoe", 1);
ToolType ToolType::sword = ToolType("Swrd", 2);
ToolType ToolType::pickaxe = ToolType("Pick", 3);
ToolType ToolType::axe = ToolType("Axe", 4);
