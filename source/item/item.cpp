#include "item.h"

#include "powergloveitem.h"
#include "resourceitem.h"
#include "toolitem.h"
#include "furnitureitem.h"
#include "../entity/workbenches/workbench.h"

typedef std::function<std::unique_ptr<Item>()> BlankConstructor;
static std::unordered_map<std::string_view, BlankConstructor> deserializeMap = {
  { "FurnitureItem", []() { return std::make_unique<FurnitureItem>(std::make_shared<Workbench>()); } },
  { "PowerGloveItem", []() { return std::make_unique<PowerGloveItem>(); } },
  { "ResourceItem", []() { return std::make_unique<ResourceItem>(Resource::cloth); } },
  { "ToolItem", []() { return std::make_unique<ToolItem>(&ToolType::pickaxe, 0); } },
};

// expects to be reading from a compound payload
std::unique_ptr<Item> Item::deserialize(std::istream& s, nbt::Tag tag) {
  return nbt::Serializable::deserialize(s, tag, deserializeMap);
}
