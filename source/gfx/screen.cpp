#include "screen.h"

#include <string>
#include "color.h"

unsigned short Screen::palette[256];
std::unique_ptr<SpriteSheet> Screen::spriteSheet = NULL;

Screen::Screen(int w, int h) : w(w), h(h) {}

void Screen::setOffset(int xOffset, int yOffset)
{
  this->xOffset = xOffset;
  this->yOffset = yOffset;
}

static std::string_view chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ      0123456789.,!?'\"-+=/\\%()<>:;     ";

void Screen::renderText(std::string_view msg, int x, int y, int col)
{
  int backgroundColor = col & 255;
  bool renderBackground = backgroundColor != 255;

  if (renderBackground)
    renderBoxFilled(x, y, msg.size() * 8, 8, backgroundColor);

  for (size_t i = 0; i < msg.length(); i++) {
    int ix = chars.find(std::toupper(msg[i]));

    if (ix >= 0) {
      renderTile(x + i * 8, y, ix + 30 * 32, col, 0);
    }
  }
}

void Screen::renderTextCentered(std::string_view msg, int x, int y, int col)
{
  renderText(msg, x - msg.size() * 4, y - 4, col);
}

void Screen::renderFrame(std::string_view title, int x0, int y0, int x1, int y1)
{
  int borderColor = Color::get(-1, 1, 5, 445);

  // corners
  renderTile(x0 * 8, y0 * 8, 0 + 13 * 32, borderColor, 0);
  renderTile(x1 * 8, y0 * 8, 0 + 13 * 32, borderColor, 1);
  renderTile(x0 * 8, y1 * 8, 0 + 13 * 32, borderColor, 2);
  renderTile(x1 * 8, y1 * 8, 0 + 13 * 32, borderColor, 3);

  // top + bottom side
  for (int y = y0 + 1; y < y1; y++) {
    renderTile(x0 * 8, y * 8, 2 + 13 * 32, borderColor, 0);
    renderTile(x1 * 8, y * 8, 2 + 13 * 32, borderColor, 1);
  }

  // left and right side
  for (int x = x0 + 1; x < x1; x++) {
    renderTile(x * 8, y0 * 8, 1 + 13 * 32, borderColor, 0);
    renderTile(x * 8, y1 * 8, 1 + 13 * 32, borderColor, 2);
  }

  // background
  renderBoxFilled((x0 + 1) * 8, (y0 + 1) * 8, (x1 - x0 - 1) * 8, (y1 - y0 - 1) * 8, Color::get(5));

  if (!title.empty()) {
    renderText(title, x0 * 8 + 8, y0 * 8, Color::get(5, 5, 5, 550));
  }
}

std::shared_ptr<Texture> Screen::genTexture(void* loadData, int w, int h)
{
  sassert(false, "Not Implemented");
  return NULL;
}

void Screen::renderTexture(Texture& texture, int x, int y, int scaleX, int scaleY)
{
  sassert(false, "Not Implemented");
}
