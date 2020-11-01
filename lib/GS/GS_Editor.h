//
// Created by matt on 2020-07-26.
//

#ifndef DOGBONE_GS_EDITOR_H
#define DOGBONE_GS_EDITOR_H

#include "GS_API.h"

#include "GS_Camera.h"

#include <GR/GR_Framebuffer.h>

#include <UT/UT_Assert.h>

namespace dogb
{
template <typename T>
void
EditorGUI(T &)
{
    UT_ASSERT_MSG(false, "GUI interface not implemented for type");
}

namespace GS
{
class DB_GS_API Editor
{
public:
    static Editor &instance();

    void initialize();
    void shutdown();

    [[nodiscard]] std::shared_ptr<GR::Framebuffer> framebuffer() const
    {
        return m_frameBuffer;
    }
private:
    std::shared_ptr<GR::Framebuffer> m_frameBuffer;
};
}

} // namespace dogb

#endif // DOGBONE_GS_EDITOR_H
