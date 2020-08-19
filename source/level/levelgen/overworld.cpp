#include "overworld.h"

#include "../tile/tile.h"
#include "noise.h"

static GeneratedLevel createOverworldLevel(Random &random, int w, int h);
static bool validateLevel(GeneratedLevel &result);

GeneratedLevel generateOverworld(Random &random, int w, int h)
{
  do
  {
    GeneratedLevel result = createOverworldLevel(random, w, h);

    if (validateLevel(result))
      return result;

  } while (true);
}

static bool validateLevel(GeneratedLevel &result)
{
  int count[256];

  for (size_t i = 0; i < result.map.size(); i++)
  {
    count[result.map[i] & 0xff]++;
  }

  if (count[Tile::rock->id & 0xff] < 100)
    return false;
  if (count[Tile::sand->id & 0xff] < 100)
    return false;
  if (count[Tile::grass->id & 0xff] < 100)
    return false;
  if (count[Tile::tree->id & 0xff] < 100)
    return false;
  if (count[Tile::stairsDown->id & 0xff] < 2)
    return false;

  return true;
}

static GeneratedLevel createOverworldLevel(Random &random, int w, int h)
{
  Noise mnoise1(random, w, h, 16);
  Noise mnoise2(random, w, h, 16);
  Noise mnoise3(random, w, h, 16);

  Noise noise1(random, w, h, 32);
  Noise noise2(random, w, h, 32);

  GeneratedLevel result;
  result.map.resize(w * h);
  result.data.resize(w * h);

  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      int i = x + y * w;

      double val = std::abs(noise1.values[i] - noise2.values[i]) * 3 - 2;
      double mval = std::abs(mnoise1.values[i] - mnoise2.values[i]);
      mval = std::abs(mval - mnoise3.values[i]) * 3 - 2;

      double xd = x / (w - 1.0) * 2 - 1;
      double yd = y / (h - 1.0) * 2 - 1;
      if (xd < 0)
        xd = -xd;
      if (yd < 0)
        yd = -yd;
      double dist = xd >= yd ? xd : yd;
      dist = dist * dist * dist * dist;
      dist = dist * dist * dist * dist;
      val = val + 1 - dist * 20;

      if (val < -0.5)
      {
        result.map[i] = Tile::water->id;
      }
      else if (val > 0.5 && mval < -1.5)
      {
        result.map[i] = Tile::rock->id;
      }
      else
      {
        result.map[i] = Tile::grass->id;
      }
    }
  }

  for (int i = 0; i < w * h / 2800; i++)
  {
    int xs = random.nextInt(w);
    int ys = random.nextInt(h);
    for (int k = 0; k < 10; k++)
    {
      int x = xs + random.nextInt(21) - 10;
      int y = ys + random.nextInt(21) - 10;
      for (int j = 0; j < 100; j++)
      {
        int xo = x + random.nextInt(5) - random.nextInt(5);
        int yo = y + random.nextInt(5) - random.nextInt(5);
        for (int yy = yo - 1; yy <= yo + 1; yy++)
          for (int xx = xo - 1; xx <= xo + 1; xx++)
            if (xx >= 0 && yy >= 0 && xx < w && yy < h)
            {
              if (result.map[xx + yy * w] == Tile::grass->id)
              {
                result.map[xx + yy * w] = Tile::sand->id;
              }
            }
      }
    }
  }

  /*
  * for (int i = 0; i < w * h / 2800; i++) { int xs = random.nextInt(w); int ys = random.nextInt(h); for (int k = 0; k < 10; k++) { int x = xs + random.nextInt(21) - 10; int y = ys + random.nextInt(21) - 10; for (int j = 0; j < 100; j++) { int xo = x + random.nextInt(5) - random.nextInt(5); int yo = y + random.nextInt(5) - random.nextInt(5); for (int yy = yo - 1; yy <= yo + 1; yy++) for (int xx = xo - 1; xx <= xo + 1; xx++) if (xx >= 0 && yy >= 0 && xx < w && yy < h) { if (map[xx + yy * w] == Tile.grass->id) { map[xx + yy * w] = Tile.dirt->id; } } } } }
  */

  for (int i = 0; i < w * h / 400; i++)
  {
    int x = random.nextInt(w);
    int y = random.nextInt(h);
    for (int j = 0; j < 200; j++)
    {
      int xx = x + random.nextInt(15) - random.nextInt(15);
      int yy = y + random.nextInt(15) - random.nextInt(15);
      if (xx >= 0 && yy >= 0 && xx < w && yy < h)
      {
        if (result.map[xx + yy * w] == Tile::grass->id)
        {
          result.map[xx + yy * w] = Tile::tree->id;
        }
      }
    }
  }

  for (int i = 0; i < w * h / 400; i++)
  {
    int x = random.nextInt(w);
    int y = random.nextInt(h);
    int col = random.nextInt(4);
    for (int j = 0; j < 30; j++)
    {
      int xx = x + random.nextInt(5) - random.nextInt(5);
      int yy = y + random.nextInt(5) - random.nextInt(5);
      if (xx >= 0 && yy >= 0 && xx < w && yy < h)
      {
        if (result.map[xx + yy * w] == Tile::grass->id)
        {
          result.map[xx + yy * w] = Tile::flower->id;
          result.data[xx + yy * w] = (char)(col + random.nextInt(4) * 16);
        }
      }
    }
  }

  for (int i = 0; i < w * h / 100; i++)
  {
    int xx = random.nextInt(w);
    int yy = random.nextInt(h);
    if (xx >= 0 && yy >= 0 && xx < w && yy < h)
    {
      if (result.map[xx + yy * w] == Tile::sand->id)
      {
        result.map[xx + yy * w] = Tile::cactus->id;
      }
    }
  }

  int count = 0;
  // stairsLoop:
  for (int i = 0; i < w * h / 100; i++)
  {
    int x = random.nextInt(w - 2) + 1;
    int y = random.nextInt(h - 2) + 1;

    bool continueStairsLoop = false;

    for (int yy = y - 1; yy <= y + 1; yy++)
    {
      for (int xx = x - 1; xx <= x + 1; xx++)
      {
        if (result.map[xx + yy * w] != Tile::rock->id)
        {
          continueStairsLoop = true;
          break;
        }
      }
      if (continueStairsLoop)
      {
        break;
      }
    }
    if (continueStairsLoop)
    {
      continue;
    }

    result.map[x + y * w] = Tile::stairsDown->id;
    count++;
    if (count == 4)
      break;
  }

  return result;
}