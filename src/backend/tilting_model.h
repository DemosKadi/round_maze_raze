//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_TILTING_MODEL_H
#define MYPROJECT_TILTING_MODEL_H

#include "position.h"
#include <algorithm>

class TiltingModel
{
public:
  constexpr TiltingModel(Position center, Position mouse_position, double max_distance)
    : center_(center), mouse_position_(mouse_position), max_distance_(max_distance)
  {}

  constexpr void mouse_position(Position pos) noexcept { mouse_position_ = pos; }
  [[nodiscard]] constexpr Position mouse_position() const noexcept { return mouse_position_; }

  constexpr void center(Position pos) noexcept { center_ = pos; }
  [[nodiscard]] constexpr Position center() const noexcept { return center_; }

  constexpr void max_distance(double distance) noexcept { max_distance_ = distance; }
  [[nodiscard]] constexpr double max_distance() const noexcept { return max_distance_; }

  [[nodiscard]] constexpr double distance() const noexcept { return center_.distance(mouse_position_); }
  [[nodiscard]] constexpr double limited_distance() const noexcept
  {
    return std::min(center_.distance(mouse_position_), max_distance_);
  }


  [[nodiscard]] constexpr double radian() const noexcept { return center_.radian(mouse_position_); }

  [[nodiscard]] constexpr Position limited_mouse_position() const noexcept
  {
    return center_.position_at(limited_distance(), radian());
  }

private:
  Position center_{};
  Position mouse_position_{};
  double max_distance_{};
};


#endif// MYPROJECT_TILTING_MODEL_H
