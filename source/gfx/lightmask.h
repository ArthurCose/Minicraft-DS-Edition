#pragma once

#include <vector>
#include "screen.h"

class Screen;

class LightMask
{
public:
  const int w, h;

  LightMask(Screen& screen);

  void reset();
  void setOffset(int xOffset, int yOffset);
  void renderLight(int x, int y, int r);
  void fillTile(int x, int y);
  void render(Screen& render);

private:
  std::vector<uint16_t> brightness;
  std::vector<std::vector<uint16_t>> precalculatedLights;
  const int brightnessW, brightnessH;
  int xOffset = 0;
  int yOffset = 0;
  std::shared_ptr<Texture> textureB;
  std::shared_ptr<Texture> textureD;
  bool usingTextureB = true;

  bool shouldBlock(int scaledX, int scaledY);
};
