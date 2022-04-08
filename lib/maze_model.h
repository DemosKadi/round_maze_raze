#ifndef MYPROBJECT_MAZE_MODEL_H
#define MYPROBJECT_MAZE_MODEL_H

#include "ball.h"
#include "vec2.h"

struct MazeModel {
    Ball ball{};
    Vec2 acceleration{};

    void tick() noexcept {
        ball.accelerate(acceleration);

        ball.tick();
    }
};

#endif
