#ifndef MYPROJECT_BALL_H
#define MYPROJECT_BALL_H

#include "position.h"

struct Ball {
    Position<double> position{};
    //! Direction in radian
    double direction{};

    //! Velocity in pixels/blocks per second
    double velocity{};

    //! acceleration: in pixels/blocks per second squared
    void apply_acceleration(double accel_direction, double acceleration) {

    }
};

#endif
