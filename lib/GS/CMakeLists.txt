project(GS)

set(sources
        GS_CameraController.cpp
        GS_SubSystem.cpp
        GS_SubSystemContext.cpp
        GS_Scene.cpp
        GS_TransformComponent.cpp
        GS_MeshComponent.cpp
        GS_Scene.cpp
        GS_Entity.cpp
        GS_EntityManager.cpp
        GS_Editor.cpp
        )

set(headers
        GS_CameraController.h
        GS_SubSystem.h
        GS_SubSystemContext.h
        GS_Scene.h
        GS_TransformComponent.h
        GS_MeshComponent.h
        GS_Scene.h
        GS_Entity.h
        GS_EntityManager.h
        GS_Editor.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC GR CONAN_PKG::entt)