#pragma once

#include <vector>
#include <memory>
#include <string_view>
#include "spritesheet.h"
#include "texture.h"

class Screen
{
protected:
  Screen(int w, int h);

public:
  static constexpr int BIT_MIRROR_X = 0x01;
  static constexpr int BIT_MIRROR_Y = 0x02;

  static unsigned short palette[256];
  static std::unique_ptr<SpriteSheet> icons;
  static std::unique_ptr<SpriteSheet> tiles;

  int xOffset = 0;
  int yOffset = 0;

  const int w, h;

  virtual void clear(int color) {};
  void setOffset(int xOffset, int yOffset);
  virtual void renderIcon(int xp, int yp, int tile, int colors, int bits) {};
  virtual void renderTile(int xp, int yp, int tile, std::array<uint8_t, 8> colors, int bits) {};
  void renderText(std::string_view msg, int x, int y, int col);
  void renderTextCentered(std::string_view msg, int x, int y, int col);
  void renderFrame(std::string_view title, int x0, int y0, int x1, int y1);
  virtual void renderPixel(int x, int y, int col) {};
  virtual void renderBox(int x, int y, int w, int h, int col) {};
  virtual void renderBoxFilled(int x, int y, int w, int h, int col) {};
  virtual std::shared_ptr<Texture> genTexture(void* loadData, int w, int h);
  virtual void renderTexture(Texture& texture, int x, int y, int scaleX, int scaleY);
};
