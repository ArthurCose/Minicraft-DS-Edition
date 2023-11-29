#include "tilepalette.h"

#include <gl2d.h>
#include "screen.h"

static unsigned short resolveColor(uint64_t compressedColors, int index)
{
  const int paletteIndex = (compressedColors >> (index * 8)) & 255;

  return Screen::palette[paletteIndex];
}

TilePalette::TilePalette(int compressedColors)
{
  glGenTextures(1, &paletteId);
  glBindTexture(0, paletteId);

  unsigned short colors[8] = {
      RGB15(0, 0, 0),
      resolveColor(compressedColors, 0),
      resolveColor(compressedColors, 1),
      resolveColor(compressedColors, 2),
      resolveColor(compressedColors, 3),
      RGB15(0, 30, 30),
      RGB15(30, 0, 30),
      RGB15(30, 20, 0),
  };

  glColorTableEXT(0, 0, 8, 0, 0, colors);
}

TilePalette::TilePalette(uint64_t compressedColors)
{
  glGenTextures(1, &paletteId);
  glBindTexture(0, paletteId);

  unsigned short colors[8] = {
      RGB15(0, 0, 0),
      resolveColor(compressedColors, 0),
      resolveColor(compressedColors, 1),
      resolveColor(compressedColors, 2),
      resolveColor(compressedColors, 3),
      resolveColor(compressedColors, 4),
      resolveColor(compressedColors, 5),
      resolveColor(compressedColors, 6),
  };

  glColorTableEXT(0, 0, 8, 0, 0, colors);
}

void TilePalette::assignToActiveTexture()
{
  glAssignColorTable(0, paletteId);
}

TilePalette::~TilePalette()
{
  sassert(false, "this should not occur");
  glDeleteTextures(1, &paletteId);
}
