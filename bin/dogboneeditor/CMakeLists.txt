project(DogBoneEditor)

set(sources
        EditorApp.cpp
        EditorWindow.cpp
        main.cpp
        GameWindow.cpp
        ProjectWindow.cpp

        TestContext_3D.cpp
        )
set(headers
        EditorApp.h
        EditorWindow.h
        GameWindow.h
        ProjectWindow.h

        TestContext_3D.h
        )

db_exec(SRCS ${sources} HEADERS ${headers})
db_link_libraries(PUBLIC DBE)

db_copy_assets(${PROJECT_NAME})
