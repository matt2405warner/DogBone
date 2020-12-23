project(GS)

set(sources
        GS_CameraController.cpp
        GS_SubSystem.cpp
        GS_Scene.cpp
        GS_TransformComponent.cpp
        GS_Mesh2DComponent.cpp
        GS_Scene.cpp
        GS_Entity.cpp
        GS_EntityManager.cpp
        GS_Editor.cpp
        GS_ComponentSystem.cpp
        GS_SystemGroup.cpp
        GS_RenderSystemGroup.cpp
        GS_World.cpp
        GS_TagComponent.cpp
        GS_CameraComponent.cpp
        GS_RenderComponentSystem.cpp
        GS_Camera.cpp
        GS_Mesh.cpp
        GS_MeshComponent.cpp
        GS_CameraControllerSystem.cpp
        GS_Material.cpp
        GS_SceneSerializer.cpp
        GS_YAML.cpp
        GS_ComponentTypeRegistry.cpp
        GS_Math.cpp
        GS_EditorCamera.cpp
        )

set(headers
        GS_CameraController.h
        GS_SubSystem.h
        GS_Scene.h
        GS_TransformComponent.h
        GS_Mesh2DComponent.h
        GS_Scene.h
        GS_Entity.h
        GS_EntityManager.h
        GS_Editor.h
        GS_ComponentSystem.h
        GS_SystemGroup.h
        GS_RenderSystemGroup.h
        GS_SystemGroupDefines.h
        GS_World.h
        GS_TagComponent.h
        GS_CameraComponent.h
        GS_RenderComponentSystem.h
        GS_Camera.h
        GS_GUITraits.h
        GS_Mesh.h
        GS_MeshComponent.h
        GS_CameraControllerSystem.h
        GS_Material.h
        GS_SceneSerializer.h
        GS_YAML.h
        GS_ComponentTypeRegistry.h
        GS_Math.h
        GS_EditorCamera.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

# At some point we need to have a clear line between runtime and editor.
db_link_libraries(PUBLIC GR IMGUI CONAN_PKG::entt)