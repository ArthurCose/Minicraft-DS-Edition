#include "underground.h"

#include "../tile/tile.h"
#include "noise.h"

static GeneratedLevel createUndergroundLevel(Random &random, int w, int h, int depth);
static bool validateLevel(GeneratedLevel &result, int depth);

GeneratedLevel generateUnderground(Random &random, int w, int h, int depth)
{
  do
  {
    GeneratedLevel result = createUndergroundLevel(random, w, h, depth);

    if (validateLevel(result, depth))
      return result;

  } while (true);
}

static bool validateLevel(GeneratedLevel &result, int depth)
{
  int count[256];

  for (size_t i = 0; i < result.map.size(); i++)
  {
    count[result.map[i] & 0xff]++;
  }

  if (count[Tile::rock->id & 0xff] < 100)
    return false;
  if (count[Tile::dirt->id & 0xff] < 100)
    return false;
  if (count[(Tile::ironOre->id & 0xff) + depth - 1] < 20)
    return false;
  if (depth < 3 && count[Tile::stairsDown->id & 0xff] < 2)
    return false;

  return true;
}

static GeneratedLevel createUndergroundLevel(Random &random, int w, int h, int depth)
{
  Noise mnoise1(random, w, h, 16);
  Noise mnoise2(random, w, h, 16);
  Noise mnoise3(random, w, h, 16);

  Noise nnoise1(random, w, h, 16);
  Noise nnoise2(random, w, h, 16);
  Noise nnoise3(random, w, h, 16);

  Noise wnoise1(random, w, h, 16);
  Noise wnoise2(random, w, h, 16);
  Noise wnoise3(random, w, h, 16);

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

      double nval = std::abs(nnoise1.values[i] - nnoise2.values[i]);
      nval = std::abs(nval - nnoise3.values[i]) * 3 - 2;

      double wval = std::abs(wnoise1.values[i] - wnoise2.values[i]);
      wval = std::abs(nval - wnoise3.values[i]) * 3 - 2;

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

      if (val > -2 && wval < -2.0 + (depth) / 2 * 3)
      {
        if (depth > 2)
          result.map[i] = Tile::lava->id;
        else
          result.map[i] = Tile::water->id;
      }
      else if (val > -2 && (mval < -1.7 || nval < -1.4))
      {
        result.map[i] = Tile::dirt->id;
      }
      else
      {
        result.map[i] = Tile::rock->id;
      }
    }
  }

  {
    int r = 2;
    for (int i = 0; i < w * h / 400; i++)
    {
      int x = random.nextInt(w);
      int y = random.nextInt(h);
      for (int j = 0; j < 30; j++)
      {
        int xx = x + random.nextInt(5) - random.nextInt(5);
        int yy = y + random.nextInt(5) - random.nextInt(5);
        if (xx >= r && yy >= r && xx < w - r && yy < h - r)
        {
          if (result.map[xx + yy * w] == Tile::rock->id)
          {
            result.map[xx + yy * w] = (char)((Tile::ironOre->id & 0xff) + depth - 1);
          }
        }
      }
    }
  }

  if (depth < 3)
  {
    int count = 0;
    // stairsLoop:
    for (int i = 0; i < w * h / 100; i++)
    {
      int x = random.nextInt(w - 20) + 10;
      int y = random.nextInt(h - 20) + 10;

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
          break;
      }

      if (continueStairsLoop)
        continue;

      result.map[x + y * w] = Tile::stairsDown->id;
      count++;
      if (count == 4)
        break;
    }
  }

  return result;
}