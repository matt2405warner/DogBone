cmake_minimum_required(VERSION 3.16)

project(DogBone VERSION "0.0.1" LANGUAGES CXX)
include(cmake/StandardProjectSettings.cmake)

set(DB_CXX_VERSION 20)
set(CMAKE_CXX_STANDARD ${DB_CXX_VERSION})

get_filename_component(DB_ROOT_DIR ${CMAKE_SOURCE_DIR} REALPATH)
if(DEFINED ENV{DB_INSTALL_PATH})
    message(STATUS "Using 'DB_INSTALL_DIRECTORY' environment variable")
    set(DB_INTERNAL_INSTALL_PATH ENV{DB_INSTALL_PATH})
else()
    set(DB_INTERNAL_INSTALL_PATH "${DB_ROOT_DIR}/build/output")
endif()
set(DB_VERSION ${PROJECT_VERSION})

# enable cache system
include(cmake/Cache.cmake)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

#option(ENABLE_TESTING "Enable Test Builds" OFF)
#option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

option(${FORCE_COLORED_OUTPUT} "Always product ANSI-coloured output (GNU/Clang Only)." TRUE)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-fdiagnostics-color=always)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif()
endif()

include(cmake/Conan.cmake)
run_conan()

if (ENABLE_TESTING)
    enable_testing()
    message("Building Tests...")
    add_subdirectory(test)
endif()

include(cmake/Targets.cmake)

add_subdirectory(lib)
add_subdirectory(bin)

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)