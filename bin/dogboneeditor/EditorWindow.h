//
// Created by matt on 2020-07-11.
//

#ifndef DOGBONE_GREDITORWINDOW_H
#define DOGBONE_GREDITORWINDOW_H

#include <GR/GR_DesktopWindow.h>
#include <GR/GR_Framebuffer.h>
#include <GR/GR_Shader.h>

#include <GS/GS_CameraController.h>

namespace dogb
{
class ProjectWindow;

class EditorWindow : public GR::DesktopWindow
{
public:
    explicit EditorWindow(const UT::WindowProperties &props)
        : GR::DesktopWindow(props)
    {
    }

    void initialize() override;

    ProjectWindow *m_projectWindow{};
    GR::ShaderLibrary m_shaderLibrary;
};

} // namespace dogb

#endif // DOGBONE_GREDITORWINDOW_H
