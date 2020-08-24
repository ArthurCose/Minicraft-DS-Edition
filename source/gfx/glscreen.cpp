#include "glscreen.h"

#include <cmath>
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

std::shared_ptr<Texture> GLScreen::genTexture(void *data, int width, int height)
{
  int widthId = std::log2(width) - 3;
  int heightId = std::log2(height) - 3;

  sassert(
      (int)std::pow(2, widthId + 3) + 1 == width && (int)std::pow(2, heightId + 3) + 1 == height &&
          widthId >= TEXTURE_SIZE_8 && widthId <= TEXTURE_SIZE_1024 &&
          heightId >= TEXTURE_SIZE_8 && heightId <= TEXTURE_SIZE_1024,
      "Texture side length must be a power of two between 8 and 1024");

  int id;
  glGenTextures(1, &id);
  glBindTexture(0, id);
  glTexImage2D(0, 0, GL_RGBA, widthId, heightId, 0, TEXGEN_OFF, data);

  return std::make_shared<Texture>(
      id,
      glGetTexturePointer(id),
      width,
      height);
}

void GLScreen::renderTexture(Texture &texture, int x, int y, int scaleX, int scaleY)
{
  x -= xOffset;
  y -= yOffset;

  glImage sprite = {
    width : texture.width,
    height : texture.height,
    u_off : 0,
    v_off : 0,
    textureID : texture.id,
  };

  int flipmode = scaleX >= 0 && scaleY >= 0 ? GL_FLIP_NONE : 0;

  if (scaleX < 0)
  {
    flipmode |= GL_FLIP_H;
    scaleX = -scaleX;
  }

  if (scaleY < 0)
  {
    flipmode |= GL_FLIP_V;
    scaleY = -scaleY;
  }

  glSpriteScaleXY(x, y, scaleY << 12, scaleY << 12, GL_FLIP_NONE, &sprite);
}
