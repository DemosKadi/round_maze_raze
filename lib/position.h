//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_POSITION_H
#define MYPROJECT_POSITION_H

//#include <gcem.hpp>
#include "vec2.h"
#include <cmath>
#include <compare>

template<typename T = int> struct Position
{
  T x{};
  T y{};

  [[nodiscard]] float distance(const Position &other) const noexcept
  {
    auto a = std::abs(x - other.x);
    auto b = std::abs(y - other.y);
    return static_cast<float>(std::sqrt(a * a + b * b));
  }

  [[nodiscard]] float radian(const Position &other) const noexcept
  {
    return static_cast<float>(std::atan2(y - other.y, x - other.x));
  }

  [[nodiscard]] Position<int> position_at(float distance, float radian) const noexcept
  {
    return {
      static_cast<int>(std::round(static_cast<float>(x) - std::cos(radian) * distance)),
      static_cast<int>(std::round(static_cast<float>(y) - std::sin(radian) * distance)),
    };
  }

  [[nodiscard]] Position apply_vec2(const Vec2 &vec2) const noexcept
  {
    return { static_cast<T>(std::round(static_cast<float>(x) + vec2.x)),
      static_cast<T>(std::round(static_cast<float>(y) + vec2.y)) };
  }

  [[nodiscard]] Position<int> rounded() const noexcept
  {
    return {
      static_cast<int>(std::round(x)),
      static_cast<int>(std::round(y))
    };
  }

  [[nodiscard]] constexpr auto operator<=>(const Position &other) const noexcept = default;
};

#endif// MYPROJECT_POSITION_H
