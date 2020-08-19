#include "screen.h"

#include <cctype>
#include <gl2d.h>
#include "color.h"

static const int SHEET_SIDE_LENGTH = 256;

unsigned short Screen::palette[256];
std::unique_ptr<SpriteSheet> Screen::spriteSheet = NULL;

Screen::Screen()
    : w(SCREEN_WIDTH), h(SCREEN_HEIGHT) {}

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
  bool renderBackground = (col & 255) != 255;
  int backgroundColor = ((col & 255) << 16) | ((col & 255) << 8);

  for (size_t i = 0; i < msg.length(); i++)
  {
    if (renderBackground)
    {
      renderTile(x + i * 8, y, 0, backgroundColor, 0);
    }

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
        renderTile(x * 8, y * 8, 0, backgroundColor, 1);
    }
  }

  renderText(title, x0 * 8 + 8, y0 * 8, Color::get(5, 5, 5, 550));
}

void Screen::renderPixel(int x, int y, int col)
{
  x -= xOffset;
  y -= yOffset;

  glPutPixel(x, y, palette[col]);
}

void Screen::renderBox(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  glBox(x, y, x + w - 1, y + h - 1, palette[col]);
}

void Screen::renderBoxFilled(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  glBoxFilled(x, y, x + w - 1, y + h - 1, palette[col]);
}
