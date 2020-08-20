#include "screen.h"

#include "color.h"

unsigned short Screen::palette[256];
std::unique_ptr<SpriteSheet> Screen::spriteSheet = NULL;

Screen::Screen(int w, int h) : w(w), h(h) {}

void Screen::setOffset(int xOffset, int yOffset)
{
  this->xOffset = xOffset;
  this->yOffset = yOffset;
}

static std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ      0123456789.,!?'\"-+=/\\%()<>:;     ";

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
