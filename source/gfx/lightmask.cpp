#include "lightmask.h"

#include <nds.h>

#include "screen.h"

static const int dither[16] = {
    0,
    8,
    2,
    10,
    12,
    4,
    14,
    6,
    3,
    11,
    1,
    9,
    15,
    7,
    13,
    5,
};

LightMask::LightMask()
    : w(SCREEN_WIDTH), h(SCREEN_HEIGHT), scale(3), brightnessW(w / scale + w % scale), brightnessH(h / scale + h % scale)
{
  brightness.resize(brightnessW * brightnessH, 0);
}

void LightMask::reset()
{
  brightness.assign(brightness.size(), 0);
}

void LightMask::setOffset(int xOffset, int yOffset)
{
  this->xOffset = xOffset;
  this->yOffset = yOffset;
}

void LightMask::renderLight(int x, int y, int r)
{
  x = (x - xOffset) / scale;
  y = (y - yOffset) / scale;
  r /= scale;

  int x0 = x - r;
  int x1 = x + r;
  int y0 = y - r;
  int y1 = y + r;

  if (x0 < 0)
    x0 = 0;
  if (y0 < 0)
    y0 = 0;
  if (x1 > brightnessW)
    x1 = brightnessW;
  if (y1 > brightnessH)
    y1 = brightnessH;

  int rSquared = r * r;

  for (int yy = y0; yy < y1; yy++)
  {
    int yd = yy - y;
    yd = yd * yd;
    for (int xx = x0; xx < x1; xx++)
    {
      int xd = xx - x;
      int dist = xd * xd + yd;

      if (dist <= rSquared)
      {
        int br = 255 - dist * 255 / rSquared;

        size_t index = xx + yy * brightnessW;

        if (brightness[index] < br)
          brightness[index] = br;
      }
    }
  }
}

void LightMask::render(Screen &screen)
{
  std::vector<DarknessBox> boxes;

  compressDarkness(boxes, 0, 0, brightnessW, brightnessH);

  for (auto &box : boxes)
  {
    screen.renderBoxFilled(box.x * scale, box.y * scale, box.w * scale, box.h * scale, 0);
  }
}

bool LightMask::shouldBlock(int scaledX, int scaledY)
{
  int brightnessIndex = scaledY * brightnessW + scaledX;
  int ditherIndex = ((scaledX + xOffset) & 3) + ((scaledY + yOffset) & 3) * 4;

  return brightness[brightnessIndex] / 10 <= dither[ditherIndex];
}

// dinky compression algorithm
// creates columns of darkness whenever a darkness pixel touches a bright pixel on the x axis
// seems to work well enough to ignore making improvements on, renderLight is the bottleneck
void LightMask::compressDarkness(
    std::vector<DarknessBox> &boxes,
    int searchX, int searchY,
    int searchW, int searchH)
{
  if (searchH == 0)
    return;

  int lastBoxR = searchX;

  int darkStartX = searchX;
  bool wasDark = false;

  for (int x = searchX; x < searchX + searchW; x++)
  {
    bool isDark = shouldBlock(x, searchY);

    if (isDark && !wasDark)
    {
      // search for boxes missed to the left
      if (x != searchX)
      {
        compressDarkness(boxes, lastBoxR, searchY + 1, x - lastBoxR, searchH - 1);
      }

      darkStartX = x;
    }

    if (!isDark && wasDark)
    {
      DarknessBox box = expandDarknessBox(darkStartX, searchY, x - darkStartX, searchH);

      boxes.push_back(box);

      lastBoxR = x;

      // search for missing boxes below this box
      compressDarkness(boxes, box.x, searchY + box.h, box.w, searchH - box.h);
    }

    wasDark = isDark;
  }

  if (wasDark)
  {
    DarknessBox box = expandDarknessBox(darkStartX, searchY, searchW - (darkStartX - searchX), searchH);

    boxes.push_back(box);

    // search for missing boxes below this box
    compressDarkness(boxes, box.x, searchY + box.h, box.w, searchH - box.h);
  }
  else
  {
    // search for boxes below
    compressDarkness(boxes, lastBoxR, searchY + 1, searchW - (lastBoxR - searchX), searchH - 1);
  }
}

DarknessBox LightMask::expandDarknessBox(
    int startX, int startY,
    int searchW, int searchH)
{
  for (int y = startY + 1; y < startY + searchH; y++)
  {
    for (int x = startX; x < startX + searchW; x++)
    {
      bool isDark = shouldBlock(x, y);

      if (!isDark)
      {
        return DarknessBox{x : startX, y : startY, w : searchW, h : y - startY};
      }
    }
  }

  return DarknessBox{x : startX, y : startY, w : searchW, h : searchH};
}
