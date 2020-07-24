project(SYS)

set(sources
        API.cpp
        SYS_Types.cpp
)

set(headers
        API.h
        SYS_Types.h
        )

db_lib(
        SRCS ${sources}
        HEADERS ${headers}
        SKIP_API
)