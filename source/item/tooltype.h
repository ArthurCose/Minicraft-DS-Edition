#pragma once

#include <string>

class ToolType
{
public:
  static ToolType shovel;
  static ToolType hoe;
  static ToolType sword;
  static ToolType pickaxe;
  static ToolType axe;

  const std::string name;
  const int sprite;

private:
  ToolType(std::string name, int sprite) : name(name), sprite(sprite) {}
};
