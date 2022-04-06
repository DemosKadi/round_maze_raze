//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_TILTING_MODEL_H
#define MYPROJECT_TILTING_MODEL_H

#include "position.h"
#include <algorithm>

class ControllerModel
{
public:
  constexpr ControllerModel(Position center_arg, Position mouse_position_arg, double max_distance_arg)
    : center(center_arg), mouse_position(mouse_position_arg), max_distance(max_distance_arg)
  {}

  [[nodiscard]] constexpr double distance() const noexcept { return center.distance(mouse_position); }
  [[nodiscard]] constexpr double limited_distance() const noexcept
  {
    return std::min(center.distance(mouse_position), max_distance);
  }

  [[nodiscard]] constexpr double radian() const noexcept { return center.radian(mouse_position); }

  [[nodiscard]] constexpr Position limited_mouse_position() const noexcept
  {
    return center.position_at(limited_distance(), radian());
  }

  Position center{};
  Position mouse_position{};
  double max_distance{};
};


#endif// MYPROJECT_TILTING_MODEL_H
