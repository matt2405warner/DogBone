//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_TESTCONTEXT_3D_H
#define DOGBONE_TESTCONTEXT_3D_H

#include <GS/GS_CameraController.h>
#include <GS/GS_SubSystemContext.h>

#include <GR/GR_Shader.h>
#include <GR/GR_Texture.h>
#include <GR/GR_VertexArray.h>
#include <GR/GR_Framebuffer.h>

#include <UT/UT_SubSystemContext.h>
#include <UT/UT_Timestep.h>

#include <glm/glm.hpp>

#include <memory>

namespace dogb::UT
{
class Window;
}

namespace dogb
{
namespace DBE
{
class SceneWindow;
}

class TestContext_3D : public GS::SubSystemContext
{
public:
    explicit TestContext_3D(DBE::SceneWindow* scene);

    void onAttach() override;

    void update(UT::Timestep ts) override;

    GR::ShaderLibrary m_shaderLibrary;
    std::shared_ptr<GR::Shader> m_texShader;
    std::shared_ptr<GR::Shader> m_flatShader;

    std::shared_ptr<GR::Texture2D> m_texture;
    std::shared_ptr<GR::Texture2D> m_blendTexture;

    std::shared_ptr<GR::VertexArray> m_sqrVAO;

    std::shared_ptr<GR::Framebuffer> m_framebuffer;

    glm::vec4 m_color;

    DBE::SceneWindow* m_sceneWindow;
};
} // namespace dogb

#endif // DOGBONE_TESTCONTEXT_3D_H
