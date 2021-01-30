//
// Created by matt on 2020-07-26.
//

#ifndef DOGBONE_GS_EDITOR_H
#define DOGBONE_GS_EDITOR_H

#include "GS_API.h"

#include "GS_Camera.h"
#include "GS_EditorCamera.h"

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
    void onUpdate(const UT::Timestep& ts);
    void shutdown();

    [[nodiscard]] std::shared_ptr<GR::Framebuffer> framebuffer() const
    {
        return m_frameBuffer;
    }

    static void setEditor(bool is_editor)
    {
        instance().m_isEditor = is_editor;
    }
    static bool isEditor()
    {
        return instance().m_isEditor;
    }
    static EditorCamera& camera()
    {
        return instance().m_camera;
    }
private:
    std::shared_ptr<GR::Framebuffer> m_frameBuffer;
    EditorCamera m_camera;
    bool m_isEditor;
};
}

} // namespace dogb

#endif // DOGBONE_GS_EDITOR_H
