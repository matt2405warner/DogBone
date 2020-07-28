project(DBE)

set(sources
    DBE_Editor.cpp
    DBE_Inspector.cpp
    DBE_SceneWindow.cpp
)

set(headers
    DBE_Editor.h
    DBE_Inspector.h
    DBE_SceneWindow.h
)

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC GR GS IMGUI)