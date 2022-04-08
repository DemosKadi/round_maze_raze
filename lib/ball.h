#ifndef MYPROJECT_BALL_H
#define MYPROJECT_BALL_H

#include "position.h"
#include "vec2.h"

struct Ball
{
    Position<float> position{};
    // velocity and direction 1 equals one block
    Vec2 velocity{};

        // apply acceleration in blocks ber second squared
    void accelerate(Vec2 acc) noexcept {
        velocity += acc;
    }
};

#endif
