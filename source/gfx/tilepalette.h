#pragma once

#include <cstdint>

class TilePalette
{
private:
  int paletteId;

public:
  TilePalette(int color);
  TilePalette(uint64_t color);

  void assignToActiveTexture();

  ~TilePalette();
};
