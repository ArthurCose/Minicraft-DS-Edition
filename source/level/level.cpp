#include <algorithm>

#include "level.h"
#include "levelgen/overworld.h"
#include "levelgen/underground.h"
#include "levelgen/sky.h"
#include "tile/tile.h"
#include "../entity/airwizard.h"
#include "../entity/slime.h"
#include "../entity/zombie.h"

Level::Level(int w, int h, int depth, Level &parentLevel)
    : Level(w, h, depth)
{
  for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++)
    {
      if (parentLevel.getTile(x, y) == Tile::stairsDown)
      {
        setTile(x, y, Tile::stairsUp->id, 0);

        if (depth == 0)
        {
          setTile(x - 1, y, Tile::hardRock->id, 0);
          setTile(x + 1, y, Tile::hardRock->id, 0);
          setTile(x, y - 1, Tile::hardRock->id, 0);
          setTile(x, y + 1, Tile::hardRock->id, 0);
          setTile(x - 1, y - 1, Tile::hardRock->id, 0);
          setTile(x - 1, y + 1, Tile::hardRock->id, 0);
          setTile(x + 1, y - 1, Tile::hardRock->id, 0);
          setTile(x + 1, y + 1, Tile::hardRock->id, 0);
        }
        else
        {
          setTile(x - 1, y, Tile::dirt->id, 0);
          setTile(x + 1, y, Tile::dirt->id, 0);
          setTile(x, y - 1, Tile::dirt->id, 0);
          setTile(x, y + 1, Tile::dirt->id, 0);
          setTile(x - 1, y - 1, Tile::dirt->id, 0);
          setTile(x - 1, y + 1, Tile::dirt->id, 0);
          setTile(x + 1, y - 1, Tile::dirt->id, 0);
          setTile(x + 1, y + 1, Tile::dirt->id, 0);
        }
      }
    }
}

Level::Level(int w, int h, int depth)
{
  if (depth < 0)
  {
    dirtColor = 222;
  }
  this->depth = depth;
  this->w = w;
  this->h = h;

  if (depth == 1)
  {
    dirtColor = 444;
  }

  GeneratedLevel generatedData;
  Random random;

  if (depth == 0)
    generatedData = generateOverworld(random, w, h);
  else if (depth < 0)
  {
    generatedData = generateUnderground(random, w, h, -depth);
    monsterDensity = 4;
  }
  else
  {
    generatedData = generateSky(random, w, h); // Sky level
    monsterDensity = 4;
  }

  tiles = generatedData.map;
  data = generatedData.data;

  entitiesInTiles.resize(w * h);

  if (depth == 1)
  {
    auto aw = std::make_shared<AirWizard>();
    aw->x = w * 8;
    aw->y = h * 8;
    add(aw);
  }
}

void Level::render(Screen &screen, Screen &lightScreen, Player &player)
{
  int xScroll = player.x - screen.w / 2;
  int yScroll = player.y - (screen.h - 8) / 2;

  if (xScroll < 16)
    xScroll = 16;
  if (yScroll < 16)
    yScroll = 16;
  if (xScroll > w * 16 - screen.w - 16)
    xScroll = w * 16 - screen.w - 16;
  if (yScroll > h * 16 - screen.h - 16)
    yScroll = h * 16 - screen.h - 16;

  if (depth < 0)
  {
    int col = Color::get(20, 20, 121, 121);
    for (int y = 0; y < 14; y++)
      for (int x = 0; x < 24; x++)
      {
        screen.renderTile(x * 8 - ((xScroll / 4) & 7), y * 8 - ((yScroll / 4) & 7), 0, col, 0);
      }
  }

  renderBackground(screen, xScroll, yScroll);
  renderSprites(screen, xScroll, yScroll);

  if (depth < 0)
  {
    lightScreen.clear(0);
    renderLight(lightScreen, xScroll, yScroll);
    screen.overlay(lightScreen, xScroll, yScroll);
  }
}

void Level::renderBackground(Screen &screen, int xScroll, int yScroll)
{
  int xo = xScroll >> 4;
  int yo = yScroll >> 4;
  int w = (screen.w + 15) >> 4;
  int h = (screen.h + 15) >> 4;
  screen.setOffset(xScroll, yScroll);
  for (int y = yo; y <= h + yo; y++)
  {
    for (int x = xo; x <= w + xo; x++)
    {
      getTile(x, y)->render(screen, *this, x, y);
    }
  }
  screen.setOffset(0, 0);
}

void Level::renderSprites(Screen &screen, int xScroll, int yScroll)
{
  int xo = xScroll >> 4;
  int yo = yScroll >> 4;
  int w = (screen.w + 15) >> 4;
  int h = (screen.h + 15) >> 4;

  screen.setOffset(xScroll, yScroll);

  std::vector<std::shared_ptr<Entity>> rowSprites;

  for (int y = yo; y <= h + yo; y++)
  {
    for (int x = xo; x <= w + xo; x++)
    {
      if (x < 0 || y < 0 || x >= this->w || y >= this->h)
        continue;

      auto entitiesInTile = entitiesInTiles[x + y * this->w];

      rowSprites.insert(rowSprites.end(), entitiesInTile.begin(), entitiesInTile.end());
    }
    if (rowSprites.size() > 0)
    {
      sortAndRender(screen, rowSprites);
      rowSprites.clear();
    }
  }

  screen.setOffset(0, 0);
}

void Level::renderLight(Screen &screen, int xScroll, int yScroll)
{
  int xo = xScroll >> 4;
  int yo = yScroll >> 4;
  int w = (screen.w + 15) >> 4;
  int h = (screen.h + 15) >> 4;

  screen.setOffset(xScroll, yScroll);
  int r = 4;
  for (int y = yo - r; y <= h + yo + r; y++)
  {
    for (int x = xo - r; x <= w + xo + r; x++)
    {
      if (x < 0 || y < 0 || x >= this->w || y >= this->h)
        continue;
      std::vector<std::shared_ptr<Entity>> entities = entitiesInTiles[x + y * this->w];
      for (auto &e : entities)
      {
        e->render(screen);
        int lr = e->getLightRadius();
        if (lr > 0)
          screen.renderLight(e->x - 1, e->y - 4, lr * 8);
      }
      int lr = getTile(x, y)->getLightRadius(*this, x, y);
      if (lr > 0)
        screen.renderLight(x * 16 + 8, y * 16 + 8, lr * 8);
    }
  }
  screen.setOffset(0, 0);
}

bool compare(std::shared_ptr<Entity> &e0, std::shared_ptr<Entity> &e1)
{
  return e1->y < e0->y;
}

void Level::sortAndRender(Screen &screen, std::vector<std::shared_ptr<Entity>> list)
{
  std::sort(list.begin(), list.end(), compare);

  for (auto &e : list)
  {
    e->render(screen);
  }
}

// Tile *Level::getTile(int x, int y)
// {
//   if (x < 0 || y < 0 || x >= w || y >= h)
//     return Tile::rock;
//   return Tile::tiles[tiles[x + y * w]];
// }

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
  if (auto playerEntity = std::dynamic_pointer_cast<Player>(entity))
  {
    player = playerEntity;
  }
  entity->removed = false;
  entities.push_back(entity);

  insertEntity(entity->x >> 4, entity->y >> 4, entity);
}

void Level::remove(std::shared_ptr<Entity> e)
{
  auto entities = entitiesInTiles[e->x + e->y * w];

  auto ePosition = std::find(entities.begin(), entities.end(), e);

  if (ePosition != entities.end())
  {
    entities.erase(ePosition);
  }

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

  auto &entitiesInTile = entitiesInTiles[x + y * w];

  auto ePosition = std::find(entitiesInTile.begin(), entitiesInTile.end(), e);

  if (ePosition != entitiesInTile.end())
  {
    entitiesInTile.erase(ePosition);
  }
}

void Level::trySpawn(int count)
{
  for (int i = 0; i < count; i++)
  {
    std::shared_ptr<Mob> mob;

    int minLevel = 1;
    int maxLevel = 1;
    if (depth < 0)
    {
      maxLevel = (-depth) + 1;
    }
    if (depth > 0)
    {
      minLevel = maxLevel = 4;
    }

    int lvl = random.nextInt(maxLevel - minLevel + 1) + minLevel;
    if (random.nextInt(2) == 0)
      mob = std::make_shared<Slime>(lvl);
    else
      mob = std::make_shared<Zombie>(lvl);

    if (mob->findStartPos(*this))
    {
      this->add(mob);
    }
  }
}

void Level::tick(Game &game)
{
  trySpawn(1);

  for (int i = 0; i < w * h / 50; i++)
  {
    int xt = random.nextInt(w);
    int yt = random.nextInt(h);
    getTile(xt, yt)->tick(*this, xt, yt);
  }

  for (size_t i = 0; i < entities.size(); i++)
  {
    auto e = entities[i];

    int xto = e->x >> 4;
    int yto = e->y >> 4;

    e->tick(game, *this, e);

    if (e->removed)
    {
      entities.erase(entities.begin() + i);
      removeEntity(xto, yto, e);
    }
    else
    {
      int xt = e->x >> 4;
      int yt = e->y >> 4;

      if (xto != xt || yto != yt)
      {
        removeEntity(xto, yto, e);
        insertEntity(xt, yt, e);
      }
    }
  }
}

std::vector<std::shared_ptr<Entity>> Level::getEntities(int x0, int y0, int x1, int y1)
{
  std::vector<std::shared_ptr<Entity>> result;
  int xt0 = (x0 >> 4) - 1;
  int yt0 = (y0 >> 4) - 1;
  int xt1 = (x1 >> 4) + 1;
  int yt1 = (y1 >> 4) + 1;
  for (int y = yt0; y <= yt1; y++)
  {
    for (int x = xt0; x <= xt1; x++)
    {
      if (x < 0 || y < 0 || x >= w || y >= h)
        continue;

      std::vector<std::shared_ptr<Entity>> entities = entitiesInTiles[x + y * this->w];

      for (auto &e : entities)
      {
        if (e->intersects(x0, y0, x1, y1))
          result.push_back(e);
      }
    }
  }
  return result;
}
