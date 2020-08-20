#include "glscreen.h"

#include <cctype>
#include <gl2d.h>
#include "color.h"

GLScreen::GLScreen()
    : Screen(SCREEN_WIDTH, SCREEN_HEIGHT) {}

void GLScreen::clear(int color)
{
  glBoxFilled(0, 0, w, h, palette[color]);
}

void GLScreen::renderTile(int xp, int yp, int tile, int colors, int bits)
{
  spriteSheet->renderTile(*this, xp, yp, tile, colors, bits);
}

void GLScreen::renderPixel(int x, int y, int col)
{
  x -= xOffset;
  y -= yOffset;

  glPutPixel(x, y, palette[col]);
}

void GLScreen::renderBox(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  glBox(x, y, x + w - 1, y + h - 1, palette[col]);
}

void GLScreen::renderBoxFilled(int x, int y, int w, int h, int col)
{
  x -= xOffset;
  y -= yOffset;

  glBoxFilled(x, y, x + w - 1, y + h - 1, palette[col]);
}
