//
// Created by matt on 2020-07-28.
//

#include "GR_Color.h"

namespace dogb::GR
{
Color &
Color::operator=(const dogb::GR::Color::parent &v)
{
    parent::operator=(v);
    return *this;
}
}
