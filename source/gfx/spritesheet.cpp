#include "spritesheet.h"

#include <gl2d.h>
#include <_ansi.h>
#include "glscreen.h"
#include "softwarescreen.h"

static unsigned short palette[8] = {
    RGB15(0, 0, 0),
    RGB15(30, 0, 0),
    RGB15(0, 30, 0),
    RGB15(0, 0, 30),
    RGB15(30, 30, 0),
    RGB15(0, 30, 30),
    RGB15(30, 0, 30),
    RGB15(30, 30, 30),
};

SpriteSheet::SpriteSheet(const unsigned char* bitmap, int width, int height, int tileW) :
  tiles((height / tileW)* (width / tileW)),
  bitmap(bitmap),
  w(width),
  h(height),
  tileW(tileW),
  textureId(
    glLoadTileSet(
      &tiles[0],                                  // pointer to glImage array
      tileW,                                      // tile width
      tileW,                                      // tile height
      width,                                      // bitmap width
      height,                                     // bitmap height
      GL_RGB16,                                   // texture type for glTexImage2D() in videoGL.h
      TEXTURE_SIZE_256,                           // sizeX for glTexImage2D() in videoGL.h
      TEXTURE_SIZE_256,                           // sizeY for glTexImage2D() in videoGL.h
      TEXGEN_OFF | GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
      8,                                          // Length of the palette to use
      palette,                                    //
      bitmap                                      // image data generated by GRIT
    )
  )
{
  sassert(width == 256 && height == 256, "Only support TEXTURE_SIZE_256 for now");
}
