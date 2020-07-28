project(DogBoneEditor)

set(sources
        ConsoleWindow.cpp
        EditorApp.cpp
        EditorWindow.cpp
        main.cpp
        GameWindow.cpp
        HierarchyWindow.cpp
        ProjectWindow.cpp

        TestSpriteComponent.cpp
        TestContext_3D.cpp
        TestComponentSystem.cpp
        )
set(headers
        ConsoleWindow.h
        EditorApp.h
        EditorWindow.h
        GameWindow.h
        HierarchyWindow.h
        ProjectWindow.h

        TestSpriteComponent.h
        TestContext_3D.h
        TestComponentSystem.h
        )

db_exec(SRCS ${sources} HEADERS ${headers})
db_link_libraries(PUBLIC DBE)

db_copy_assets(${PROJECT_NAME})
