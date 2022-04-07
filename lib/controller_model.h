//
// Created by tom on 03.04.22.
//

#ifndef MYPROJECT_TILTING_MODEL_H
#define MYPROJECT_TILTING_MODEL_H

#include "position.h"
#include "vec2.h"
#include <algorithm>

class ControllerModel
{
public:
  ControllerModel(Position<> center_arg, Position<> mouse_position_arg, float max_distance_arg)
    : center(center_arg), mouse_position(mouse_position_arg), max_distance(max_distance_arg)
  {}

  [[nodiscard]] float distance() const noexcept { return center.distance(mouse_position); }
  [[nodiscard]] float limited_distance() const noexcept
  {
    return std::min(center.distance(mouse_position), max_distance);
  }

  [[nodiscard]] float radian() const noexcept { return center.radian(mouse_position); }

  [[nodiscard]] Position<> limited_mouse_position() const noexcept
  {
    return center.position_at(limited_distance(), radian());
  }


  [[nodiscard]] Vec2 limited_direction() const noexcept
  {
    return Vec2{ static_cast<float>(mouse_position.x - center.x), static_cast<float>(mouse_position.y - center.y) }
             .normalize()
           * max_distance;
  }

  Position<> center{};
  Position<> mouse_position{};
  float max_distance{};
};


#endif// MYPROJECT_TILTING_MODEL_H
