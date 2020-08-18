#include "screen.h"

#include <cctype>
#include <gl2d.h>
#include "color.h"

static const int SHEET_SIDE_LENGTH = 256;

unsigned short Screen::palette[256];
std::unique_ptr<SpriteSheet> Screen::spriteSheet = NULL;

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

Screen::Screen(int w, int h)
    : w(w), h(h)
{
  pixels.resize(w * h, 0);
}

void Screen::clear(int color)
{
  glBoxFilled(0, 0, w, h, palette[color]);
}

void Screen::setOffset(int xOffset, int yOffset)
{
  this->xOffset = xOffset;
  this->yOffset = yOffset;
}

void Screen::renderTile(int xp, int yp, int tile, int colors, int bits)
{
  spriteSheet->renderTile(*this, xp, yp, tile, colors, bits);
}

std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ      0123456789.,!?'\"-+=/\\%()<>:;     ";

void Screen::renderText(std::string msg, int x, int y, int col)
{
  for (size_t i = 0; i < msg.length(); i++)
  {
    int ix = chars.find(std::toupper(msg[i]));
    if (ix >= 0)
    {
      renderTile(x + i * 8, y, ix + 30 * 32, col, 0);
    }
  }
}

void Screen::renderTextCentered(std::string msg, int x, int y, int col)
{
  renderText(msg, x - msg.size() * 4, y - 4, col);
}

void Screen::renderFrame(std::string title, int x0, int y0, int x1, int y1)
{
  int borderColor = Color::get(-1, 1, 5, 445);
  int backgroundColor = Color::get(5, 5, 5, 5);

  for (int y = y0; y <= y1; y++)
  {
    for (int x = x0; x <= x1; x++)
    {
      if (x == x0 && y == y0)
        renderTile(x * 8, y * 8, 0 + 13 * 32, borderColor, 0);
      else if (x == x1 && y == y0)
        renderTile(x * 8, y * 8, 0 + 13 * 32, borderColor, 1);
      else if (x == x0 && y == y1)
        renderTile(x * 8, y * 8, 0 + 13 * 32, borderColor, 2);
      else if (x == x1 && y == y1)
        renderTile(x * 8, y * 8, 0 + 13 * 32, borderColor, 3);
      else if (y == y0)
        renderTile(x * 8, y * 8, 1 + 13 * 32, borderColor, 0);
      else if (y == y1)
        renderTile(x * 8, y * 8, 1 + 13 * 32, borderColor, 2);
      else if (x == x0)
        renderTile(x * 8, y * 8, 2 + 13 * 32, borderColor, 0);
      else if (x == x1)
        renderTile(x * 8, y * 8, 2 + 13 * 32, borderColor, 1);
      else
        renderTile(x * 8, y * 8, 2 + 13 * 32, backgroundColor, 1);
    }
  }

  renderText(title, x0 * 8 + 8, y0 * 8, Color::get(5, 5, 5, 550));
}

void Screen::renderLight(int x, int y, int r)
{
  // todo: hardware rendering

  x -= xOffset;
  y -= yOffset;
  int x0 = x - r;
  int x1 = x + r;
  int y0 = y - r;
  int y1 = y + r;

  if (x0 < 0)
    x0 = 0;
  if (y0 < 0)
    y0 = 0;
  if (x1 > w)
    x1 = w;
  if (y1 > h)
    y1 = h;

  for (int yy = y0; yy < y1; yy++)
  {
    int yd = yy - y;
    yd = yd * yd;
    for (int xx = x0; xx < x1; xx++)
    {
      int xd = xx - x;
      int dist = xd * xd + yd;

      if (dist <= r * r)
      {
        int br = 255 - dist * 255 / (r * r);

        if (pixels[xx + yy * w] < br)
          glPutPixel(xx, yy, palette[br]);
      }
    }
  }
}

void Screen::overlay(Screen &screen2, int xa, int ya)
{
  // todo: hardware rendering
  int i = 0;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      if (screen2.pixels[i] / 10 <= dither[((x + xa) & 3) + ((y + ya) & 3) * 4])
        pixels[i] = palette[0];
      i++;
    }
  }
}
