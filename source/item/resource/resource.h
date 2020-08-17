#pragma once

#include <string>

class Tile;
class Level;
class Player;

class Resource
{
public:
  static Resource wood;
  static Resource stone;
  static Resource flower;
  static Resource acorn;
  static Resource dirt;
  static Resource sand;
  static Resource cactusFlower;
  static Resource seeds;
  static Resource wheat;
  static Resource bread;
  static Resource apple;

  static Resource coal;
  static Resource ironOre;
  static Resource goldOre;
  static Resource ironIngot;
  static Resource goldIngot;

  static Resource slime;
  static Resource glass;
  static Resource cloth;
  static Resource cloud;
  static Resource gem;

  const std::string name;
  const int sprite;
  const int color;

  Resource(std::string name, int sprite, int color);

  virtual bool interactOn(Tile &tile, Level &level, int xt, int yt, Player &player, int attackDir) { return false; }
};
