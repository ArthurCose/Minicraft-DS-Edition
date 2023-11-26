#include "level.h"

#include <algorithm>

#include "levelgen/overworld.h"
#include "levelgen/underground.h"
#include "levelgen/sky.h"
#include "tile/tile.h"
#include "../entity/hostile/airwizard.h"
#include "../entity/hostile/slime.h"
#include "../entity/hostile/zombie.h"
#include "../gfx/lightmask.h"
#include "../nbt.h"

Level::Level(int w, int h, int depth, Level& parentLevel)
  : Level(w, h, depth)
{
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
      if (parentLevel.getTile(x, y) == Tile::stairsDown) {
        setTile(x, y, Tile::stairsUp, 0);

        if (depth == 0) {
          setTile(x - 1, y, Tile::hardRock, 0);
          setTile(x + 1, y, Tile::hardRock, 0);
          setTile(x, y - 1, Tile::hardRock, 0);
          setTile(x, y + 1, Tile::hardRock, 0);
          setTile(x - 1, y - 1, Tile::hardRock, 0);
          setTile(x - 1, y + 1, Tile::hardRock, 0);
          setTile(x + 1, y - 1, Tile::hardRock, 0);
          setTile(x + 1, y + 1, Tile::hardRock, 0);
        } else {
          setTile(x - 1, y, Tile::dirt, 0);
          setTile(x + 1, y, Tile::dirt, 0);
          setTile(x, y - 1, Tile::dirt, 0);
          setTile(x, y + 1, Tile::dirt, 0);
          setTile(x - 1, y - 1, Tile::dirt, 0);
          setTile(x - 1, y + 1, Tile::dirt, 0);
          setTile(x + 1, y - 1, Tile::dirt, 0);
          setTile(x + 1, y + 1, Tile::dirt, 0);
        }
      }
    }
}

Level::Level(int w, int h, int depth)
{
  if (depth < 0) {
    dirtColor = 222;
  }
  this->depth = depth;
  this->w = w;
  this->h = h;

  if (depth == 1) {
    dirtColor = 444;
  }

  GeneratedLevel generatedData;

  if (depth == 0) {
    generatedData = generateOverworld(Random::globalRandom, w, h);
  } else if (depth < 0) {
    generatedData = generateUnderground(Random::globalRandom, w, h, -depth);
  } else {
    generatedData = generateSky(Random::globalRandom, w, h); // Sky level
  }

  tiles = generatedData.map;
  data = generatedData.data;
  tickRate = generatedData.tickRate;
  monsterDensity = generatedData.monsterDensity = 4;

  map = std::make_shared<std::vector<unsigned char>>();
  map->resize(w * h, depth < 0 ? Color::get(0) : Color::get(451));

  entitiesInTiles.resize(w * h);

  if (depth == 1) {
    auto aw = std::make_shared<AirWizard>();
    aw->x = w * 8;
    aw->y = h * 8;
    add(aw);
  }
}

void Level::tick(Game& game)
{
  trySpawn(1);

  int bitW = std::ceil(std::log2(w));
  int bitH = std::ceil(std::log2l(h));
  int totalBlockTicks = fixed32(w * h) * tickRate;

  for (int i = 0; i < totalBlockTicks; i++) {
    int r = random.nextBits(bitW + bitH);
    int xt = r % w;
    int yt = (r >> bitW) % h;
    Tile::tiles[getTile(xt, yt)]->tick(*this, xt, yt);
  }

  for (size_t i = 0; i < entities.size(); i++) {
    auto e = entities[i];

    int xto = e->x >> 4;
    int yto = e->y >> 4;

    e->tick(game, *this, e);

    if (e->removed) {
      std::iter_swap(entities.begin() + i, entities.end() - 1);
      entities.pop_back();
      removeEntity(xto, yto, e);
      i--;
    } else {
      int xt = e->x >> 4;
      int yt = e->y >> 4;

      if (xto != xt || yto != yt) {
        removeEntity(xto, yto, e);
        insertEntity(xt, yt, e);
      }
    }
  }

  updateMap(
    game.player->x >> 4,
    game.player->y >> 4,
    depth < 0 ? game.player->getLightRadius() : 8);
}

void Level::updateMap(int mapX, int mapY, int viewDistance)
{
  int left = mapX - viewDistance;
  int right = mapX + viewDistance + 1;
  int top = mapY - viewDistance;
  int bottom = mapY + viewDistance + 1;

  left = std::clamp(left, 0, w);
  right = std::clamp(right, 0, w);
  top = std::clamp(top, 0, h);
  bottom = std::clamp(bottom, 0, h);

  auto& m = *map;

  for (int y = top; y < bottom; y++)
    for (int x = left; x < right; x++)
      m[y * w + x] = Tile::tiles[getTile(x, y)]->getMapColor(*this, x, y);
}

void Level::render(Screen& screen, LightMask& lightMask, Player& player)
{
  int xScroll = player.x - screen.w / 2;
  int yScroll = player.y - (screen.h - 8) / 2;

  xScroll = std::clamp(xScroll, 16, w * 16 - screen.w - 16);
  yScroll = std::clamp(yScroll, 16, h * 16 - screen.h - 16);

  renderBackground(screen, xScroll, yScroll);
  renderSprites(screen, xScroll, yScroll);

  if (depth < 0) {
    lightMask.reset();
    renderLight(lightMask, xScroll, yScroll);
    lightMask.render(screen);
  }
}

void Level::renderBackground(Screen& screen, int xScroll, int yScroll)
{
  if (depth == 1) {
    int col = Color::get(20, 20, 121, 121);

    for (int y = 0; y < screen.h / 8 + 1; y++) {
      for (int x = 0; x < screen.w / 8 + 1; x++) {
        int screenX = x * 8 - ((xScroll / 4) & 7);
        int screenY = y * 8 - ((yScroll / 4) & 7);
        int worldX = screenX + xScroll;
        int worldY = screenY + yScroll;

        bool covered =
          getTile((worldX + 0) >> 4, (worldY + 0) >> 4) != Tile::infiniteFall &&
          getTile((worldX + 0) >> 4, (worldY + 8) >> 4) != Tile::infiniteFall &&
          getTile((worldX + 8) >> 4, (worldY + 8) >> 4) != Tile::infiniteFall &&
          getTile((worldX + 8) >> 4, (worldY + 0) >> 4) != Tile::infiniteFall;

        if (!covered)
          screen.renderTile(screenX, screenY, 0, col, 0);
      }
    }
  }

  int xo = xScroll >> 4;
  int yo = yScroll >> 4;
  int w = (screen.w + 15) >> 4;
  int h = (screen.h + 15) >> 4;
  screen.setOffset(xScroll, yScroll);
  for (int y = yo; y <= h + yo; y++) {
    for (int x = xo; x <= w + xo; x++) {
      Tile::tiles[getTile(x, y)]->render(screen, *this, x, y);
    }
  }
  screen.setOffset(0, 0);
}

void Level::renderSprites(Screen& screen, int xScroll, int yScroll)
{
  int w = (screen.w + 15) >> 4;
  int h = (screen.h + 15) >> 4;
  int x0 = std::max(xScroll >> 4, 0);
  int y0 = std::max(yScroll >> 4, 0);
  int x1 = std::min(x0 + w, this->w - 1);
  int y1 = std::min(y0 + h, this->h - 1);

  screen.setOffset(xScroll, yScroll);

  std::vector<std::shared_ptr<Entity>> rowSprites;

  for (int y = y0; y <= y1; y++) {
    for (int x = x0; x <= x1; x++) {
      auto& entitiesInTile = entitiesInTiles[x + y * this->w];

      rowSprites.insert(rowSprites.end(), entitiesInTile.begin(), entitiesInTile.end());
    }

    if (rowSprites.size() > 0) {
      sortAndRender(screen, rowSprites);
      rowSprites.clear();
    }
  }

  screen.setOffset(0, 0);
}

void Level::renderLight(LightMask& lightMask, int xScroll, int yScroll)
{
  int xo = xScroll >> 4;
  int yo = yScroll >> 4;
  int w = (lightMask.w + 15) >> 4;
  int h = (lightMask.h + 15) >> 4;

  lightMask.setOffset(xScroll, yScroll);
  int r = 4;

  int x0 = std::max(xo - r, 0);
  int y0 = std::max(yo - r, 0);
  int x1 = std::min(w + xo + r, this->w - 1);
  int y1 = std::min(h + yo + r, this->h - 1);

  for (int y = y0; y <= y1; y++) {
    for (int x = x0; x <= x1; x++) {
      auto& entities = entitiesInTiles[x + y * this->w];

      for (auto& e : entities) {
        int lr = e->getLightRadius();

        if (lr > 0)
          lightMask.renderLight(e->x - 1, e->y - 4, lr * 8);
      }

      int lr = Tile::tiles[getTile(x, y)]->getLightRadius(*this, x, y);

      if (lr > 0) {
        bool aboveLit = Tile::tiles[getTile(x, y - 1)]->getLightRadius(*this, x, y - 1) > 1;
        bool belowLit = Tile::tiles[getTile(x, y + 1)]->getLightRadius(*this, x, y + 1) > 1;
        bool leftLit = Tile::tiles[getTile(x - 1, y)]->getLightRadius(*this, x - 1, y) > 1;
        bool rightLit = Tile::tiles[getTile(x + 1, y)]->getLightRadius(*this, x + 1, y) > 1;

        bool verticalNeighborsLit = aboveLit && belowLit;
        bool horizontalNeighborsLit = leftLit && rightLit;
        bool neighborsLit = verticalNeighborsLit && horizontalNeighborsLit;

        if (neighborsLit) {
          lightMask.fillTile(x, y);
        } else if ((horizontalNeighborsLit && x % 2) || (verticalNeighborsLit && y % 2)) {
          // edge tile, but our neighbors also provide light
          // skip every other
        } else {
          lightMask.renderLight(x * 16 + 8, y * 16 + 8, lr * 8);
        }
      }
    }
  }
}

static bool compareY(std::shared_ptr<Entity>& e0, std::shared_ptr<Entity>& e1)
{
  return e0->y < e1->y;
}

void Level::sortAndRender(Screen& screen, std::vector<std::shared_ptr<Entity>> list)
{
  std::sort(list.begin(), list.end(), compareY);

  for (auto& e : list) {
    e->render(screen);
  }
}

unsigned char Level::getTile(int x, int y)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return Tile::rock;
  return tiles[x + y * w];
}

void Level::setTile(int x, int y, unsigned char tile, int dataVal)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return;
  tiles[x + y * w] = tile;
  data[x + y * w] = (unsigned char)dataVal;
}

int Level::getData(int x, int y)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return 0;
  return data[x + y * w] & 0xff;
}

void Level::setData(int x, int y, int val)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return;
  data[x + y * w] = (unsigned char)val;
}

void Level::add(std::shared_ptr<Entity> entity)
{
  if (auto playerEntity = std::dynamic_pointer_cast<Player>(entity)) {
    player = playerEntity;
  }
  entity->removed = false;
  entities.push_back(entity);
  Entity::associateId(entity);

  insertEntity(entity->x >> 4, entity->y >> 4, entity);
}

void Level::remove(std::shared_ptr<Entity> e)
{
  auto iter = std::find(entities.begin(), entities.end(), e);

  if (iter != entities.end())
    entities.erase(iter);

  int xto = e->x >> 4;
  int yto = e->y >> 4;
  removeEntity(xto, yto, e);
}

void Level::insertEntity(int x, int y, std::shared_ptr<Entity> e)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return;
  entitiesInTiles[x + y * w].push_back(e);
}

void Level::removeEntity(int x, int y, std::shared_ptr<Entity> e)
{
  if (x < 0 || y < 0 || x >= w || y >= h)
    return;

  auto& entitiesInTile = entitiesInTiles[x + y * w];
  auto iter = std::find(entitiesInTile.begin(), entitiesInTile.end(), e);

  if (iter != entitiesInTile.end())
    entitiesInTile.erase(iter);
}

void Level::trySpawn(int count)
{
  for (int i = 0; i < count; i++) {
    std::shared_ptr<Mob> mob;

    int minLevel = 1;
    int maxLevel = 1;
    if (depth < 0) {
      maxLevel = (-depth) + 1;
    }
    if (depth > 0) {
      minLevel = maxLevel = 4;
    }

    int lvl = random.nextInt(maxLevel - minLevel + 1) + minLevel;
    if (random.nextInt(2) == 0)
      mob = std::make_shared<Slime>(lvl);
    else
      mob = std::make_shared<Zombie>(lvl);

    if (mob->findStartPos(*this)) {
      this->add(mob);
    }
  }
}

int Level::countEntities(int x0, int y0, int x1, int y1)
{
  int result = 0;
  int xt0 = std::max((x0 >> 4) - 1, 0);
  int yt0 = std::max((y0 >> 4) - 1, 0);
  int xt1 = std::min((x1 >> 4) + 1, w - 1);
  int yt1 = std::max((y1 >> 4) + 1, h - 1);

  for (int y = yt0; y <= yt1; y++) {
    for (int x = xt0; x <= xt1; x++) {
      auto& entities = entitiesInTiles[x + y * this->w];

      result += entities.size();
    }
  }

  return result;
}

std::vector<std::shared_ptr<Entity>> Level::getEntities(int x0, int y0, int x1, int y1)
{
  std::vector<std::shared_ptr<Entity>> result;
  int xt0 = std::max((x0 >> 4) - 1, 0);
  int yt0 = std::max((y0 >> 4) - 1, 0);
  int xt1 = std::min((x1 >> 4) + 1, w - 1);
  int yt1 = std::max((y1 >> 4) + 1, h - 1);

  for (int y = yt0; y <= yt1; y++) {
    for (int x = xt0; x <= xt1; x++) {
      auto& entities = entitiesInTiles[x + y * this->w];

      for (auto& e : entities) {
        if (e->intersects(x0, y0, x1, y1))
          result.push_back(e);
      }
    }
  }

  return result;
}

// expects to write into a compound payload
void Level::serialize(std::ostream& s)
{
  nbt::write_named_int(s, "Depth", depth);
  nbt::write_named_int(s, "Width", w);
  nbt::write_named_int(s, "Height", h);
  nbt::write_named_int(s, "GrassColor", grassColor);
  nbt::write_named_int(s, "DirtColor", dirtColor);
  nbt::write_named_int(s, "SandColor", sandColor);
  nbt::write_named_int(s, "MonsterDensity", monsterDensity);
  nbt::write_named_float(s, "TickRate", tickRate);
  nbt::write_named_byte_array(s, "Tiles", (const char*)&tiles[0], tiles.size());
  nbt::write_named_byte_array(s, "TileData", (const char*)&data[0], data.size());
  nbt::write_named_byte_array(s, "MapData", (const char*)&(*map)[0], (*map).size());

  nbt::begin_named_list(s, "Entities", nbt::Tag::COMPOUND, entities.size());

  for (auto& entity : entities) {
    entity->serialize(s);
    nbt::close_compound(s);
  }
}

// expects to be reading from a compound payload
Level Level::deserialize(std::istream& s)
{
  Level level;

  std::vector<std::shared_ptr<Entity>> entities;

  nbt::read_tagged_compound(s, nbt::Tag::COMPOUND, [&level, &entities, &s](nbt::Tag tag, std::string name) {
    if (name == "Depth") {
      level.depth = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "Width") {
      level.w = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "Height") {
      level.h = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "GrassColor") {
      level.grassColor = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "DirtColor") {
      level.dirtColor = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "SandColor") {
      level.sandColor = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "MonsterDensity") {
      level.monsterDensity = nbt::read_tagged_number<int>(s, tag);
    } else if (name == "TickRate") {
      level.tickRate = nbt::read_tagged_number<float>(s, tag);
    } else if (name == "Tiles") {
      level.tiles = nbt::read_byte_array<unsigned char>(s);
    } else if (name == "TileData") {
      level.data = nbt::read_byte_array<unsigned char>(s);
    } else if (name == "MapData") {
      level.map = std::make_shared<std::vector<unsigned char>>(
        nbt::read_byte_array<unsigned char>(s)
      );
    } else if (name == "Entities") {
      nbt::read_tagged_list(s, tag, nbt::Tag::COMPOUND, [&entities, &s](int i) {
        if (auto entity = Entity::deserialize(s, nbt::Tag::COMPOUND)) {
          entities.push_back(std::move(entity));
        }
      });
    } else {
      nbt::skip_tag_payload(s, tag);
    }
  });

  // enforce consistent dimensions for tile related vectors
  size_t totalTiles = level.w * level.h;
  level.entitiesInTiles.resize(totalTiles);
  level.tiles.resize(totalTiles);
  level.data.resize(totalTiles);
  level.map->resize(totalTiles);

  // add entities now that entitiesInTiles has the proper size
  for (auto entity : entities) {
    level.add(entity);
  }

  return level;
}