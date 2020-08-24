#include "texture.h"

#include <nds.h>
#include <nds/arm9/videoGL.h>

Texture::Texture(int id, void *const data, int width, int height)
    : data(data), width(width), height(height)
{
  this->id = id;
}

Texture::~Texture()
{
  if (data >= VRAM_A && data < VRAM_E)
  {
    glDeleteTextures(1, &id);
  }
}
