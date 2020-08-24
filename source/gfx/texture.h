#pragma once

struct Texture
{
  int id;
  void *const data;
  const int width;
  const int height;

  // should not be used directly
  Texture(int id, void *const data, int width, int height);

  ~Texture();
};
