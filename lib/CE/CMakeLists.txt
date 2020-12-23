project(CE)

set(sources
        CE_Input.cpp
        CE_GLFWHelpers.cpp
        CE_Asset.cpp
        CE_Archiver.cpp
        CE_HotKeyManager.cpp
        )

set(headers
        CE_Input.h
        CE_GLFWHelpers.h
        CE_Asset.h
        CE_Archiver.h
        CE_HotKeyManager.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC UT CONAN_PKG::glfw CONAN_PKG::glad CONAN_PKG::yaml-cpp)