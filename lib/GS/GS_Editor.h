//
// Created by matt on 2020-07-26.
//

#ifndef DOGBONE_GS_EDITOR_H
#define DOGBONE_GS_EDITOR_H

#include "GS_API.h"

#include <rttr/type>

#include <glm/glm.hpp>

#include <UT/UT_Assert.h>
#include <iostream>
#include <type_traits>

namespace dogb
{
template <typename T>
void
EditorGUI(T &)
{
    UT_ASSERT_MSG(false, "GUI interface not implemented for type");
}
} // namespace dogb

#endif // DOGBONE_GS_EDITOR_H
