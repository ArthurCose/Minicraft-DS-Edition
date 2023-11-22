#pragma once

#include <string>

class Tile;
class Level;
class Player;

class Resource
{
public:
  enum ID
  {
    wood,
    stone,
    flower,
    acorn,
    dirt,
    sand,
    cactusFlower,
    seeds,
    wheat,
    bread,
    apple,
    coal,
    ironOre,
    goldOre,
    ironIngot,
    goldIngot,
    slime,
    glass,
    cloth,
    cloud,
    gem,
  };

  static Resource* resources[256];

  const std::string name;
  const int sprite;
  const int color;

  Resource(std::string name, int sprite, int color);

  virtual bool interactOn(Tile& tile, Level& level, int xt, int yt, Player& player, int attackDir) const { return false; }
};
