project(IMGUI)

set(sources
        IMGUI_Window.cpp
        IMGUI_SubSystem.cpp
        IMGUI_SubSystemContext.cpp

        IMGUI_OpenGLImpl.cpp
        IMGUI_GLFWImpl.cpp
        )

set(headers
        IMGUI_Window.h
        IMGUI_SubSystem.h
        IMGUI_SubSystemContext.h

        IMGUI_OpenGLImpl.h
        IMGUI_GLFWImpl.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC GR CONAN_PKG::imgui)