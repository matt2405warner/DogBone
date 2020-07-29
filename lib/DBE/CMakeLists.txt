project(DBE)

set(sources
    DBE_Editor.cpp
    DBE_Inspector.cpp
    DBE_SceneWindow.cpp
    DBE_HierarchyWindow.cpp
    DBE_EditorIMGUIContext.cpp
)

set(headers
    DBE_Editor.h
    DBE_Inspector.h
    DBE_SceneWindow.h
    DBE_HierarchyWindow.h
    DBE_EditorIMGUIContext.h
)

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC GR GS IMGUI)