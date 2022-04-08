#ifndef MYPROJECT_VEC2_H
#define MYPROJECT_VEC2_H

#include <cmath>

struct Vec2
{
  float x{};
  float y{};

  [[nodiscard]] float magnitude() const noexcept { return std::sqrt(x * x + y * y); }

  [[nodiscard]] Vec2 normalize() const noexcept
  {
    if (x == 0.0F && y == 0.0F) { return *this; }
    auto m = magnitude();
    return { x / m, y / m };
  }

  [[nodiscard]] Vec2 operator*(float value) const noexcept { return { x * value, y * value }; }

  [[nodiscard]] Vec2 operator+(const Vec2 &other) const noexcept { return { x + other.x, y * other.y }; }

  Vec2 &operator+=(const Vec2 &other) noexcept
  {
    *this = *this + other;
    return *this;
  }
};

#endif
