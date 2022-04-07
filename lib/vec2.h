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
    auto m = magnitude();
    return { x / m, y / m };
  }

  [[nodiscard]] Vec2 operator*(float value) const noexcept { return { x * value, y * value }; }
};

#endif