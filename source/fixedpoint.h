#pragma once

#include <cstdint>
#include <nds/arm9/math.h>
#include <nds/arm9/trig_lut.h>

/// @brief DS standard fixed point with 12 bit fractional value and 32 bits total
class fixed32 {
private:
  int32_t v{};

public:
  constexpr fixed32() {}

  constexpr fixed32(int n) {
    v = intToFixed(n, 12);
  }

  constexpr fixed32(float n) {
    v = floatToFixed(n, 12);
  }

  constexpr fixed32(double n) {
    v = floatToFixed(n, 12);
  }

  inline fixed32 sin();
  inline fixed32 cos();

  fixed32 abs() const {
    fixed32 f;
    f.v = std::abs(v);
    return f;
  }

  fixed32 operator+() const {
    return *this;
  }

  fixed32 operator-() const {
    fixed32 f = *this;
    f.v = -f.v;
    return f;
  }

  fixed32 operator+(fixed32 const& o) const {
    fixed32 f;
    f.v = v + o.v;
    return f;
  }

  fixed32& operator+=(fixed32 const& o) {
    v += o.v;
    return *this;
  }

  fixed32 operator-(fixed32 const& o) const {
    fixed32 f;
    f.v = v - o.v;
    return f;
  }

  fixed32& operator-=(fixed32 const& o) {
    v -= o.v;
    return *this;
  }

  fixed32 operator*(fixed32 const& o) const {
    fixed32 f;
    f.v = mulf32(v, o.v);
    return f;
  }

  fixed32& operator*=(fixed32 const& o) {
    v = mulf32(v, o.v);
    return *this;
  }

  fixed32 operator/(fixed32 const& o) const {
    fixed32 f;
    f.v = divf32(v, o.v);
    return f;
  }

  fixed32 operator/=(fixed32 const& o) {
    v = divf32(v, o.v);
    return *this;
  }

  auto operator<(fixed32 const& b) const { return v < b.v; }
  auto operator<=(fixed32 const& b) const { return v <= b.v; }
  auto operator>(fixed32 const& b) const { return v > b.v; }
  auto operator>=(fixed32 const& b) const { return v >= b.v; }
  auto operator==(fixed32 const& b) const { return v == b.v; }
  auto operator!=(fixed32 const& b) const { return v != b.v; }

  auto operator<(int const& b) const { return *this < fixed32(b); }
  auto operator<=(int const& b) const { return *this <= fixed32(b); }
  auto operator>(int const& b) const { return *this > fixed32(b); }
  auto operator>=(int const& b) const { return *this >= fixed32(b); }
  auto operator==(int const& b) const { return *this == fixed32(b); }
  auto operator!=(int const& b) const { return *this != fixed32(b); }

  operator int() const {
    return fixedToInt(v, 12);
  }

  operator float() const {
    return fixedToFloat(v, 12);
  }

  int raw() const {
    return v;
  }
};

inline auto operator<(int const& a, fixed32 const& b) { return fixed32(a) < b; }
inline auto operator<=(int const& a, fixed32 const& b) { return fixed32(a) <= b; }
inline auto operator>(int const& a, fixed32 const& b) { return fixed32(a) > b; }
inline auto operator>=(int const& a, fixed32 const& b) { return fixed32(a) >= b; }
inline auto operator==(int const& a, fixed32 const& b) { return fixed32(a) == b; }
inline auto operator!=(int const& a, fixed32 const& b) { return fixed32(a) != b; }

constexpr fixed32 FIXED_RAD_TO_ANGLE = 180.0 / M_PI * DEGREES_IN_CIRCLE / 360.0;

fixed32 fixed32::sin() {
  auto angle = *this * FIXED_RAD_TO_ANGLE;
  fixed32 f;
  f.v = sinLerp((int)angle);
  return f;
}

fixed32 fixed32::cos() {
  auto angle = *this * FIXED_RAD_TO_ANGLE;
  fixed32 f;
  f.v = cosLerp((int)angle);
  return f;
}