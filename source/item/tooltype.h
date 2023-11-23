#pragma once

#include <string>
#include <array>

class ToolType
{
public:
  static ToolType shovel;
  static ToolType hoe;
  static ToolType sword;
  static ToolType pickaxe;
  static ToolType axe;

  const std::string name;
  std::array<std::string, 5> levelNames;
  const int sprite;

private:
  ToolType(std::string name, int sprite);
};
