project(UT)

set(sources
        UT_Application.cpp
        UT_Logger.cpp
        UT_IDManager.cpp
        UT_Window.cpp
        UT_SubSystem.cpp
        UT_SubSystemContext.cpp
        UT_Engine.cpp
        UT_EventDispatcher.cpp
        UT_Timestep.cpp
        UT_Profiler.cpp
        )

set(headers
        UT_Application.h
        UT_Logger.h
        UT_IDManager.h
        UT_Window.h
        UT_SubSystem.h
        UT_SubSystemContext.h
        UT_Engine.h
        UT_EventDispatcher.h
        UT_Timestep.h
        UT_Profiler.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC SYS CONAN_PKG::spdlog CONAN_PKG::fmt CONAN_PKG::rttr CONAN_PKG::boost)