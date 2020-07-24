if (CMAKE_DB_PATH)
    return()
endif()

set(CMAKE_DB_PATH TRUE)

if (NOT DB_INTERNAL_INSTALL_PATH)
    message(FATAL_ERROR "Cannot properly setup paths without an install path set!")
endif()

set(CMAKE_INSTALL_PREFIX ${DB_INTERNAL_INSTALL_PATH} CACHE PATH "CMake Install Prefix" FORCE)

# Header Include path
set(DB_INCLUDE_PATH_NAME include)
set(DB_INCLUDE_PATH "${DB_INTERNAL_INSTALL_PATH}/${DB_INCLUDE_PATH_NAME}")

# Runtime output path information
set(DB_BIN_INSTALL_PATH_NAME bin)
set(DB_BIN_INSTALL_PATH "${DB_INTERNAL_INSTALL_PATH}/${DB_BIN_INSTALL_PATH_NAME}")

# Library output path information
set(DB_LIB_INSTALL_PATH_NAME lib)
set(DB_LIB_PATH "${DB_INTERNAL_INSTALL_PATH}/${DB_LIB_INSTALL_PATH_NAME}")

# Generated files output path
set(DB_GEN_INSTALL_PATH_NAME gen)
set(DB_GEN_INSTALL_PATH "${DB_INTERNAL_INSTALL_PATH}/${DB_GEN_INSTALL_PATH_NAME}")

# Determine the cmake prefix path
set(CMAKE_PREFIX_PATH "${DB_LIB_PATH}/cmake/" CACHE PATH "CMake Prefix Path" FORCE)

# CMake module path
set(DB_CMAKE_MODULE_PATH "${DB_ROOT_DIR}/cmake")

# Config path
set(RN_CONFIG_PATH ${CMAKE_PREFIX_PATH})

message("Module Path: ${CMAKE_MODULE_PATH}")