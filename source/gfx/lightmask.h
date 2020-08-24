#pragma once

#include <vector>
#include "screen.h"

class Screen;

struct DarknessBox
{
  int x;
  int y;
  int w;
  int h;
};

class LightMask
{
public:
  const int w, h;
  const int scale;

  int xOffset = 0;
  int yOffset = 0;

  LightMask(Screen &screen);

  void reset();
  void setOffset(int xOffset, int yOffset);
  void renderLight(int x, int y, int r);
  void updateTexture();
  void render(Screen &render);

private:
  std::vector<unsigned char> brightness;
  const int brightnessW, brightnessH;
  std::shared_ptr<Texture> textureB;
  std::shared_ptr<Texture> textureD;
  bool usingTextureB = true;

  bool shouldBlock(int scaledX, int scaledY);
};
