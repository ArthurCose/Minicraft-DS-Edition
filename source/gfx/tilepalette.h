#pragma once

class TilePalette
{
private:
  int paletteId;

public:
  TilePalette(int color);

  void assignToActiveTexture();

  ~TilePalette();
};
