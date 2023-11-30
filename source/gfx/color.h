#pragma once

#include <memory>
#include <nds.h>

class Color
{
public:
  static inline constexpr int get(int a, int b, int c, int d)
  {
    return (get(d) << 24) + (get(c) << 16) + (get(b) << 8) + (get(a));
  }

  static inline constexpr uint8_t get(int d)
  {
    if (d < 0)
      return 255;

    uint8_t r = d / 100 % 10;
    uint8_t g = d / 10 % 10;
    uint8_t b = d % 10;

    return r * 36 + g * 6 + b;
  }

  template<size_t size>
  static inline constexpr std::array<uint8_t, size> getArray(std::initializer_list<int> numbers)
  {
    std::array<uint8_t, size> result = {};
    size_t i = 0;

    for (auto& n : numbers) {
      if (i >= size) {
        break;
      }

      result[i] = Color::get(n);
      i += 1;
    }

    return result;
  }
};
