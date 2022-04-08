#ifndef MYPROJECT_BALL_H
#define MYPROJECT_BALL_H

#include "position.h"
#include "vec2.h"

struct Ball
{
  Position<float> position{};
  // velocity and direction 1 equals one block
  // velocity in blocks per tick
  Vec2 velocity{};

  // apply acceleration in blocks ber tick squared
  void accelerate(Vec2 acc) noexcept { velocity += acc; }

  void tick() noexcept { position = position.apply_vec2(velocity); }
};

#endif
