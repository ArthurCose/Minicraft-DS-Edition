#include "sky.h"

#include "../tile/tile.h"
#include "noise.h"

static GeneratedLevel createSkyLevel(Random &random, int w, int h);
static bool validateLevel(GeneratedLevel &result);

GeneratedLevel generateSky(Random &random, int w, int h)
{
  do
  {
    GeneratedLevel result = createSkyLevel(random, w, h);

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

  if (count[Tile::cloud & 0xff] < 2000)
    return false;
  if (count[Tile::stairsDown & 0xff] < 2)
    return false;

  return true;
}

static GeneratedLevel createSkyLevel(Random &random, int w, int h)
{
  Noise noise1(random, w, h, 8);
  Noise noise2(random, w, h, 8);

  GeneratedLevel result;
  result.map.resize(w * h);
  result.data.resize(w * h);

  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      int i = x + y * w;

      double val = std::abs(noise1.values[i] - noise2.values[i]) * 3 - 2;

      double xd = x / (w - 1.0) * 2 - 1;
      double yd = y / (h - 1.0) * 2 - 1;
      if (xd < 0)
        xd = -xd;
      if (yd < 0)
        yd = -yd;
      double dist = xd >= yd ? xd : yd;
      dist = dist * dist * dist * dist;
      dist = dist * dist * dist * dist;
      val = -val * 1 - 2.2;
      val = val + 1 - dist * 20;

      if (val < -0.25)
      {
        result.map[i] = Tile::infiniteFall;
      }
      else
      {
        result.map[i] = Tile::cloud;
      }
    }
  }

  // stairsLoop:
  for (int i = 0; i < w * h / 50; i++)
  {
    int x = random.nextInt(w - 2) + 1;
    int y = random.nextInt(h - 2) + 1;

    bool continueStairsLoop = false;

    for (int yy = y - 1; yy <= y + 1; yy++)
      for (int xx = x - 1; xx <= x + 1; xx++)
      {
        if (result.map[xx + yy * w] != Tile::cloud)
        {
          continueStairsLoop = true;
          break;
        }

        if (continueStairsLoop)
          break;
      }

    if (continueStairsLoop)
      continue;

    result.map[x + y * w] = Tile::cloudCactus;
  }

  int count = 0;

  // stairsLoop:
  for (int i = 0; i < w * h; i++)
  {
    int x = random.nextInt(w - 2) + 1;
    int y = random.nextInt(h - 2) + 1;

    bool continueStairsLoop = false;

    for (int yy = y - 1; yy <= y + 1; yy++)
    {
      for (int xx = x - 1; xx <= x + 1; xx++)
      {
        if (result.map[xx + yy * w] != Tile::cloud)
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

    result.map[x + y * w] = Tile::stairsDown;

    count++;

    if (count == 2)
      break;
  }

  return result;
}
