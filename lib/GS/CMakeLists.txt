project(GS)

set(sources
        GS_CameraController.cpp
        GS_SubSystem.cpp
        GS_SubSystemContext.cpp
        )

set(headers
        GS_CameraController.h
        GS_SubSystem.h
        GS_SubSystemContext.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
)

db_link_libraries(PUBLIC GR)