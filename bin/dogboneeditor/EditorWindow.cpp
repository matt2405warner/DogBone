//
// Created by matt on 2020-07-11.
//

#include "EditorWindow.h"

#include "GameWindow.h"
#include "ProjectWindow.h"

#include <DBE/DBE_ConsoleWindow.h>
#include <DBE/DBE_EditorIMGUIContext.h>
#include <DBE/DBE_HierarchyWindow.h>
#include <DBE/DBE_Inspector.h>
#include <DBE/DBE_SceneWindow.h>
#include <DBE/DBE_Editor.h>

#include <IMGUI/IMGUI_SubSystem.h>

#include <GS/GS_CameraComponent.h>
#include <GS/GS_CameraController.h>
#include <GS/GS_Mesh2DComponent.h>
#include <GS/GS_MeshComponent.h>
#include <GS/GS_World.h>
#include <GS/GS_Material.h>

#include <UT/UT_Assert.h>

namespace dogb
{
void
EditorWindow::initialize()
{
    GR::DesktopWindow::initialize();

    DBE::Editor::addMenuCallback("File/Test", [](){
    });
    DBE::Editor::addMenuCallback("Entity/Create Empty", []() {
        GS::World &world = GS::World::instance();
        GS::Entity e = world.createEntity();
        if (world.m_selectedEntity)
        {
            world.m_selectedEntity.addChildEntity(e);
        }
        else
        {
            world.m_selectedEntity = e;
        }
    });
    DBE::Editor::addMenuCallback("Component/Add Component", [](){

    });
    DBE::Editor::addMenuCallback("Window/Show/Scene Window", [](){

    });
    DBE::Editor::addMenuCallback("Help/About", [](){

    });

    GR::Framebuffer::Specification spec;
    spec.m_width = m_width;
    spec.m_height = m_height;

    GS::World &world = GS::World::instance();
    world.mainCamera()->m_activeTexture = GR::Framebuffer::create(spec);
    world.mainCamera()->setViewportSize(m_width, m_height);

    auto imgui_ctx =
            addContext<IMGUI::SubSystem, DBE::EditorIMGUIContext>(this);
    UT_ASSERT(imgui_ctx != nullptr);

    // Add all of the standard editor GUI windows
    DBE::Inspector *inspector = imgui_ctx->createGUIWindow<DBE::Inspector>();
    inspector->show();
    ProjectWindow *proj_window = imgui_ctx->createGUIWindow<ProjectWindow>();
    proj_window->show();

    DBE::HierarchyWindow *hierarchy =
            imgui_ctx->createGUIWindow<DBE::HierarchyWindow>();
    hierarchy->show();

    DBE::SceneWindow *scene_win =
            imgui_ctx->createGUIWindow<DBE::SceneWindow>();
    scene_win->show();

    GameWindow *game_win = imgui_ctx->createGUIWindow<GameWindow>();
    game_win->show();

    DBE::ConsoleWindow *console_win =
            imgui_ctx->createGUIWindow<DBE::ConsoleWindow>();
    console_win->show();

    auto ent = world.createEntity();
    ent.addComponent<GS::Mesh2DComponent>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
    // m_testEntity.addComponent<TestComponent>(5);
    world.createEntity();
    world.createEntity();
    auto cam_ent = world.createEntity();
    GS::CameraComponent &cam_comp = cam_ent.addComponent<GS::CameraComponent>();
    cam_comp.m_camera = world.mainCamera();
    cam_ent.addComponent<GS::CameraController>();

    auto ent_3d = world.createEntity();
    GS::MeshComponent& mesh_comp = ent_3d.addComponent<GS::MeshComponent>();

    auto material = std::make_shared<GS::Material>();
    material->m_mainTexture = GR::Texture2D::create("../assets/textures/stones.jpg");

    material->m_shader = m_shaderLibrary.load("../assets/shaders/Test3D.glsl");

    mesh_comp.m_mesh.m_material = material;
    mesh_comp.m_mesh.m_VAO = GR::VertexArray::create();

    /* clang-format off */
    float sqr_vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
            -0.5,  0.5f,  0.0f, 0.0f, 1.0f
    };
    /* clang-format on */
    std::shared_ptr<GR::VertexBuffer> sqr_vb(
            GR::VertexBuffer::create(sqr_vertices, sizeof(sqr_vertices)));
    GR::BufferLayout sqr_layout{
            {GR::ShaderDataType::Float3, "a_Position"},
            {GR::ShaderDataType::Float2, "a_TexCoord"}};
    sqr_vb->setLayout(sqr_layout);
    mesh_comp.m_mesh.m_VAO->addVertexBuffer(sqr_vb);
    uint32_t sqr_indices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<GR::IndexBuffer> sqr_ibuffer(GR::IndexBuffer::create(
            sqr_indices, sizeof(sqr_indices) / sizeof(uint32_t)));
    mesh_comp.m_mesh.m_VAO->setIndexBuffer(sqr_ibuffer);

    // Test Mesh Comp2
    auto material2 = std::make_shared<GS::Material>();
    material2->m_mainTexture = GR::Texture2D::create("../assets/textures/Checkerboard.png");
    material2->m_shader = material->m_shader;
    auto ent_3d_2 = world.createEntity();
    GS::MeshComponent& mesh_comp_2 = ent_3d_2.addComponent<GS::MeshComponent>();
    mesh_comp_2.m_mesh.m_VAO = mesh_comp.m_mesh.m_VAO;
    mesh_comp_2.m_mesh.m_material = material2;


    world.m_selectedEntity = ent;
}

} // namespace dogb