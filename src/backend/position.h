//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_POSITION_H
#define MYPROJECT_POSITION_H

#include <compare>
#include <gcem.hpp>

template<typename T = int> struct Position
{
  T x{};
  T y{};

  [[nodiscard]] constexpr double distance(const Position &other) const noexcept
  {
    auto a = gcem::abs(x - other.x);
    auto b = gcem::abs(y - other.y);
    return gcem::sqrt(a * a + b * b);
  }

  [[nodiscard]] constexpr double radian(const Position &other) const noexcept
  {
    return gcem::atan2(y - other.y, x - other.x);
  }

  [[nodiscard]] constexpr Position position_at(double distance, double radian) const noexcept
  {
    return {
      static_cast<T>(gcem::round(x - gcem::cos(radian) * distance)),
      static_cast<T>(gcem::round(y - gcem::sin(radian) * distance)),
    };
  }

  [[nodiscard]] constexpr auto operator<=>(const Position &other) const noexcept = default;

  [[nodiscard]] constexpr Position<int> rounded() const noexcept
  {
    return { static_cast<int>(gcem::round(x)), static_cast<int>(gcem::round(y)) };
  };
};

#endif// MYPROJECT_POSITION_H
