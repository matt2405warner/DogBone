//
// Created by matt on 2020-08-10.
//

#ifndef DOGBONE_IMGUI_MODAL_H
#define DOGBONE_IMGUI_MODAL_H

#include "IMGUI_API.h"

namespace dogb::IMGUI
{
class DB_IMGUI_API Modal
{
public:
    virtual ~Modal() = default;

    [[nodiscard]] inline bool isOpen() const { return m_open; }

    virtual void render() = 0;
protected:
    Modal();

    bool m_open;
};
}

#endif // DOGBONE_IMGUI_MODAL_H
