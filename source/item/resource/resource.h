#pragma once

#include <string>

class Tile;
class Level;
class Player;

class Resource
{
public:
  static const Resource *wood;
  static const Resource *stone;
  static const Resource *flower;
  static const Resource *acorn;
  static const Resource *dirt;
  static const Resource *sand;
  static const Resource *cactusFlower;
  static const Resource *seeds;
  static const Resource *wheat;
  static const Resource *bread;
  static const Resource *apple;

  static const Resource *coal;
  static const Resource *ironOre;
  static const Resource *goldOre;
  static const Resource *ironIngot;
  static const Resource *goldIngot;

  static const Resource *slime;
  static const Resource *glass;
  static const Resource *cloth;
  static const Resource *cloud;
  static const Resource *gem;

  const std::string name;
  const int sprite;
  const int color;

  Resource(std::string name, int sprite, int color);

  virtual bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) const { return false; }
};
