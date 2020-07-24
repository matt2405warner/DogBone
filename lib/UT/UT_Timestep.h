//
// Created by matt on 2020-07-12.
//

#ifndef DOGBONE_UT_TIMESTEP_H
#define DOGBONE_UT_TIMESTEP_H

#include "UT_API.h"

namespace dogb::UT
{
class UT_API Timestep
{
public:
    Timestep(float t = 0.0f) :
        m_time(t)
    {}

    [[nodiscard]] float seconds() const { return m_time; }
    [[nodiscard]] float milliseconds() const { return m_time * 1000.0f; }
private:
    float m_time;
};
}

#endif // DOGBONE_UT_TIMESTEP_H
