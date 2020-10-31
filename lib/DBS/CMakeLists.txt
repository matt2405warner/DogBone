project(DBS)

set(sources
    DBS_NativeScriptComponent.cpp
    DBS_NativeScriptSystem.cpp
    DBS_ScriptableEntity.cpp
)

set(headers
    DBS_NativeScriptComponent.h
    DBS_NativeScriptSystem.h
    DBS_ScriptableEntity.h
)

db_lib(
    SRCS ${sources}
    HEADERS ${headers}
)

db_link_libraries(
        PUBLIC GS
)