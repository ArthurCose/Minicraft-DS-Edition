#pragma once

class Player;
class Mob;

#include <memory>
#include "../../shim/random.h"

class Screen;
class Level;
class Entity;
class Item;

class Tile
{
protected:
  static constexpr Random& random = Random::globalRandom;
public:
  static int tickCount;

  static Tile* tiles[256]; // raw pointers for static addresses
  enum ID
  {
    grass,
    rock,
    water,
    flower,
    tree,
    dirt,
    sand,
    cactus,
    hole,
    treeSapling,
    cactusSapling,
    farmland,
    wheat,
    lava,
    stairsDown,
    stairsUp,
    infiniteFall,
    cloud,
    hardRock,
    ironOre,
    goldOre,
    gemOre,
    cloudCactus,
  };

  const unsigned char id;

  bool connectsToGrass = false;
  bool connectsToSand = false;
  bool connectsToLava = false;
  bool connectsToWater = false;

  Tile(unsigned char id);

  virtual void render(Screen& screen, Level& level, int x, int y) {};

  virtual bool mayPass(Level& level, int x, int y, Entity& e) { return true; };

  virtual int getMapColor(Level& level, int x, int y) { return 404; };

  virtual int getLightRadius(Level& level, int x, int y) { return 0; };

  virtual void hurt(Level& level, int x, int y, Mob& source, int dmg, int attackDir) {};

  virtual void bumpedInto(Level& level, int xt, int yt, Entity& entity) {};

  virtual void tick(Level& level, int xt, int yt) {};

  virtual void steppedOn(Level& level, int xt, int yt, Entity& entity) {};

  virtual bool interact(Level& level, int xt, int yt, Player& player, Item& item, int attackDir)
  {
    return false;
  }

  virtual bool use(Level& level, int xt, int yt, Player& player, int attackDir)
  {
    return false;
  }

  bool connectsToLiquid()
  {
    return connectsToWater || connectsToLava;
  }
};
