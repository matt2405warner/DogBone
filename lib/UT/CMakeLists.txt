project(UT)

set(sources
        UT_Assert.cpp
        UT_Application.cpp
        UT_Debug.cpp
        UT_Logger.cpp
        UT_IDManager.cpp
        UT_Window.cpp
        UT_SubSystem.cpp
        UT_EventDispatcher.cpp
        UT_Timestep.cpp
        UT_Profiler.cpp
        UT_FileDialog.cpp
        )

set(headers
        UT_Assert.h
        UT_Application.h
        UT_Debug.h
        UT_Logger.h
        UT_IDManager.h
        UT_Window.h
        UT_SubSystem.h
        UT_EventDispatcher.h
        UT_Timestep.h
        UT_Profiler.h
        UT_FileDialog.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC SYS CONAN_PKG::spdlog CONAN_PKG::fmt CONAN_PKG::rttr CONAN_PKG::boost)