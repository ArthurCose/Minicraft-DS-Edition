#pragma once

#include <vector>
#include <memory>
#include "tile/tile.h"
#include "../game.h"
#include "../entity/player.h"
#include "../entity/entity.h"
#include "../shim/random.h"
#include "levelgen/generatedlevel.h"

class Screen;
class LightMask;

class Level
{
protected:
  static constexpr Random& random = Random::globalRandom;
private:
  int depth;

  void updateMap(int x, int y, int viewDistance);
  void sortAndRender(Screen& screen, std::vector<std::shared_ptr<Entity>> list);
  void insertEntity(int x, int y, std::shared_ptr<Entity> e);
  void removeEntity(int x, int y, std::shared_ptr<Entity> e);
  void renderBackground(Screen& screen, int xScroll, int yScroll);
  void renderSprites(Screen& screen, int xScroll, int yScroll);
  void renderLight(LightMask& screen, int xScroll, int yScroll);

public:
  int w, h;
  int grassColor = 141;
  int dirtColor = 322;
  int sandColor = 550;
  int monsterDensity = GeneratedLevel::DEFAULT_MONSTER_DENSITY;
  fixed32 tickRate = GeneratedLevel::DEFAULT_TICK_RATE;
  std::shared_ptr<std::vector<unsigned char>> map;
  std::vector<unsigned char> tiles;
  std::vector<unsigned char> data;
  std::vector<std::shared_ptr<Entity>> entities;
  std::vector<std::vector<std::shared_ptr<Entity>>> entitiesInTiles;
  std::shared_ptr<Player> player;

  Level() {};
  Level(int w, int h, int level);
  Level(int w, int h, int level, Level& parentLevel);

  void tick(Game& game);
  void render(Screen& screen, LightMask& LightMask, Player& player);
  unsigned char getTile(int x, int y);
  void setTile(int x, int y, unsigned char tile, int dataVal);
  int getData(int x, int y);
  void setData(int x, int y, int val);
  void add(std::shared_ptr<Entity> entity);
  void remove(std::shared_ptr<Entity> e);
  void trySpawn(int count);

  int countEntities(int x0, int y0, int x1, int y1);
  std::vector<std::shared_ptr<Entity>> getEntities(int x0, int y0, int x1, int y1);
  void serialize(std::ostream& s);
  static Level deserialize(std::istream& s);
};
