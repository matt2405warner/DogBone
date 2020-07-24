//
// Created by matt on 2020-07-14.
//

#ifndef DOGBONE_TESTCONTEXT_2D_H
#define DOGBONE_TESTCONTEXT_2D_H

#include <GS/GS_SubSystemContext.h>

#include <GR/GR_Texture.h>
#include <GR/GR_Framebuffer.h>

#include <UT/UT_SubSystemContext.h>
#include <UT/UT_Timestep.h>

#include <memory>

namespace dogb
{
class SceneWindow;

class TestContext_2D : public GS::SubSystemContext
{
public:
    explicit TestContext_2D(SceneWindow* scene_window);

    void onAttach() override;

    void update(UT::Timestep ts) override ;


    std::shared_ptr<GR::Texture2D> m_texture;
    std::shared_ptr<GR::Framebuffer> m_framebuffer;

    SceneWindow* m_sceneWindow;
};

}

#endif // DOGBONE_TESTCONTEXT_2D_H
