project(DogBoneEditor)

set(sources
        ConsoleWindow.cpp
        Editor.cpp
        EditorWindow.cpp
        main.cpp
        GameWindow.cpp
        HierarchyWindow.cpp
        Inspector.cpp
        ProjectWindow.cpp
        SceneWindow.cpp

        TestContext_3D.cpp
        TestContext_2D.cpp
        )
set(headers
        ConsoleWindow.h
        Editor.h
        EditorWindow.h
        GameWindow.h
        HierarchyWindow.h
        Inspector.h
        ProjectWindow.h
        SceneWindow.h

        TestContext_3D.h
        TestContext_2D.h
        )

db_exec(SRCS ${sources} HEADERS ${headers})
db_link_libraries(PUBLIC UT GR IMGUI GS)

db_copy_assets(${PROJECT_NAME})
