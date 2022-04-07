//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_POSITION_H
#define MYPROJECT_POSITION_H

//#include <gcem.hpp>
#include <compare>
#include <cmath>

template<typename T = int>
struct Position
{
  int x{};
  int y{};

  [[nodiscard]] float distance(const Position &other) const noexcept
  {
    auto a = std::abs(x - other.x);
    auto b = std::abs(y - other.y);
    return std::sqrt(a * a + b * b);
  }

  [[nodiscard]] float radian(const Position &other) const noexcept
  {
    return std::atan2(y - other.y, x - other.x);
  }

  [[nodiscard]] Position position_at(float distance, float radian) const noexcept
  {
    return {
      static_cast<int>(std::round(x - std::cos(radian) * distance)),
      static_cast<int>(std::round(y - std::sin(radian) * distance)),
    };
  }

  [[nodiscard]] constexpr auto operator<=>(const Position &other) const noexcept = default;
};

#endif// MYPROJECT_POSITION_H
