#pragma once

#include <vector>
#include <memory>
#include <string>
#include "screen.h"
#include "spritesheet.h"

class GLScreen : public Screen
{
public:
  GLScreen();

  void clear(int color) override;
  void renderIcon(int xp, int yp, int tile, int colors, int bits) override;
  void renderTile(int xp, int yp, int tile, std::array<uint8_t, 8> colors, int bits) override;
  void renderPixel(int x, int y, int col) override;
  void renderBox(int x, int y, int w, int h, int col) override;
  void renderBoxFilled(int x, int y, int w, int h, int col) override;
  std::shared_ptr<Texture> genTexture(void* loadData, int w, int h);
  void renderTexture(Texture& texture, int x, int y, int scaleX, int scaleY) override;
  int totalPalettes();
  int glSpriteCallCount();
  void flush();

private:
  static std::vector<int> iconPaletteColors;
  static std::vector<std::shared_ptr<TilePalette>> iconPalettes;
  static std::vector<uint64_t> tilePaletteColors;
  static std::vector<std::shared_ptr<TilePalette>> tilePalettes;

  int glSpriteCalls = 0;
  int lastIconColors = -1;
  uint64_t lastTileColors = -1;
};
