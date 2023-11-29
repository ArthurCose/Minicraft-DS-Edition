#include "glscreen.h"

#include <cmath>
#include <gl2d.h>
#include "color.h"

std::vector<int> GLScreen::iconPaletteColors;
std::vector<std::shared_ptr<TilePalette>> GLScreen::iconPalettes;
std::vector<uint64_t> GLScreen::tilePaletteColors;
std::vector<std::shared_ptr<TilePalette>> GLScreen::tilePalettes;

GLScreen::GLScreen()
  : Screen(SCREEN_WIDTH, SCREEN_HEIGHT) {}

void GLScreen::clear(int color)
{
  glBoxFilled(0, 0, w, h, palette[color]);
}

template<typename CompressedColors>
static inline std::shared_ptr<TilePalette> resolvePalette(std::vector<std::shared_ptr<TilePalette>>& palettes, std::vector<CompressedColors>& paletteColors, CompressedColors compressedColors)
{
  std::shared_ptr<TilePalette> palette;

  auto lowerBound = std::lower_bound(paletteColors.begin(), paletteColors.end(), compressedColors);

  if (lowerBound == paletteColors.end()) {
    palette = std::make_shared<TilePalette>(compressedColors);
    palettes.push_back(palette);
    paletteColors.push_back(compressedColors);
  } else if (*lowerBound != compressedColors) {
    palette = std::make_shared<TilePalette>(compressedColors);
    palettes.insert(palettes.begin() + std::distance(paletteColors.begin(), lowerBound), palette);
    paletteColors.insert(lowerBound, compressedColors);
  } else {
    palette = palettes[std::distance(paletteColors.begin(), lowerBound)];
  }

  return palette;
}

void GLScreen::renderIcon(int xp, int yp, int tile, int compressedColors, int bits)
{
  xp -= xOffset;
  yp -= yOffset;

  if (lastIconColors != compressedColors) {
    auto palette = resolvePalette(iconPalettes, iconPaletteColors, compressedColors);
    glSetActiveTexture(icons->textureId);
    palette->assignToActiveTexture();

    lastIconColors = compressedColors;
  }

  // convert to GL_FLIP_H and GL_FLIP_V
  int flipMode = ((bits & Screen::BIT_MIRROR_X) << 2) | (bits & Screen::BIT_MIRROR_Y);

  glSprite(xp, yp, flipMode, icons->tileGlImage(tile));
  glSpriteCalls += 1;
}

void GLScreen::renderTile(int xp, int yp, int tile, std::array<uint8_t, 8> colors, int bits)
{
  xp -= xOffset;
  yp -= yOffset;

  uint64_t compressedColors = *(uint64_t*)&colors[0];

  if (lastTileColors != compressedColors) {
    auto palette = resolvePalette(tilePalettes, tilePaletteColors, compressedColors);
    glSetActiveTexture(tiles->textureId);
    palette->assignToActiveTexture();

    lastTileColors = compressedColors;
  }

  // convert to GL_FLIP_H and GL_FLIP_V
  int flipMode = ((bits & Screen::BIT_MIRROR_X) << 2) | (bits & Screen::BIT_MIRROR_Y);

  glSprite(xp, yp, flipMode, tiles->tileGlImage(tile));
  glSpriteCalls += 1;
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

std::shared_ptr<Texture> GLScreen::genTexture(void* data, int width, int height)
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

void GLScreen::renderTexture(Texture& texture, int x, int y, int scaleX, int scaleY)
{
  x -= xOffset;
  y -= yOffset;

  glImage sprite = {
    width: texture.width,
    height : texture.height,
    u_off : 0,
    v_off : 0,
    textureID : texture.id,
  };

  int flipmode = scaleX >= 0 && scaleY >= 0 ? GL_FLIP_NONE : 0;

  if (scaleX < 0) {
    flipmode |= GL_FLIP_H;
    scaleX = -scaleX;
  }

  if (scaleY < 0) {
    flipmode |= GL_FLIP_V;
    scaleY = -scaleY;
  }

  glSpriteScaleXY(x, y, scaleY << 12, scaleY << 12, GL_FLIP_NONE, &sprite);
}

int GLScreen::totalPalettes()
{
  return iconPalettes.size() + tilePalettes.size();
}

int GLScreen::glSpriteCallCount()
{
  return glSpriteCalls;
}

void GLScreen::flush()
{
  glFlush(0);
  glSpriteCalls = 0;;
}
