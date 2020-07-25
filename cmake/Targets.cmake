if (CMAKE_DB_TARGETS)
    return()
endif()

set(CMAKE_DB_TARGETS TRUE)

include(CMakePackageConfigHelpers)

include(cmake/Path.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

# ===
# Function for installing static/shared libraries
# ===

function(db_install is_static)
    set(version_config "${lib_config_dir}/${target_lib_name}ConfigVersion.cmake")
    set(project_config_out "${lib_config_dir}/${target_lib_name}Config.cmake")

    if (${is_static})
        set(target_config_file "${DB_CMAKE_MODULE_PATH}/StaticConfig.cmake.in")
    else()
        set(target_config_file "${DB_CMAKE_MODULE_PATH}/SharedConfig.cmake.in")
    endif()

    # Write out a basic package version file and install it in the library
    # configuration dir
    write_basic_package_version_file(
            ${version_config}
            VERSION ${DB_VERSION}
            COMPATIBILITY SameMajorVersion
    )

    # Write out a basic configuration file and install it in the library
    # configuration folder
    configure_package_config_file(
            ${target_config_file}
            ${project_config_out}
            INSTALL_DESTINATION ${lib_config_dir}
    )

    # Export out files
    install(
            TARGETS ${target_name}_Headers;${target_lib_name}
            EXPORT ${target_lib_name}Targets
            LIBRARY DESTINATION ${DB_LIB_INSTALL_PATH_NAME}
            ARCHIVE DESTINATION ${DB_LIB_INSTALL_PATH_NAME}
            RUNTIME DESTINATION ${DB_BIN_INSTALL_PATH_NAME}
            INCLUDES DESTINATION ${DB_INCLUDE_PATH_NAME}/${target_name}
    )

    # Export our targets
    install(
            EXPORT ${target_lib_name}Targets
            FILE ${target_lib_name}Targets.cmake
            NAMESPACE DOGBONE::
            DESTINATION ${CMAKE_PREFIX_PATH}
    )

    # We need this so that we can skip over in source build when calling
    # `find_package()` which allows us to export all libraries and easily link
    # to them within our projects
    add_library(DOGBONE::${target_lib_name} ALIAS ${target_lib_name})
endfunction()

macro(copy_headers command_target headers)
    #file(INSTALL ${headers} DESTINATION "${DB_INCLUDE_PATH}/${target_name}")
    if (NOT EXISTS "${DB_INCLUDE_PATH}/${target_name}")
        add_custom_command(
                TARGET ${command_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory "${DB_INCLUDE_PATH}/${target_name}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                "Creating include directory for ${command_target}"
        )
    endif()
    add_custom_command(
            TARGET ${command_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${all_headers} "${DB_INCLUDE_PATH}/${target_name}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Copy headers for target ${command_target}"
    )
endmacro()

macro(db_set_compile_definitions name)
    if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime to generate time tracing .json files on clang" OFF)
        if (ENABLE_BUILD_WITH_TIME_TRACE)
            add_compile_definitions(${name} INTERFACE -ftime-trace)
        endif()
    endif()

    set(all_defines "")

    # Determine build type
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        list(APPEND all_defines "DB_RELEASE")
        list(APPEND all_defines "DB_ASSERT_LEVEL=0")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND all_defines "DB_DEBUG")
        list(APPEND all_defines "DB_ASSERT_LEVEL=1")
    endif()

    # determine platform type
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        list(APPEND all_defines "DB_WINDOWS")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "CYGWIN")
        list(APPEND all_defines "DB_WINDOWS")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        list(APPEND all_defines "DB_LINUX")
    else()
        message(FATAL_ERROR "Unsupported system type: ${CMAKE_SYSTEM_NAME}")
    endif()

    add_compile_definitions(${name} INTERFACE ${all_defines})
endmacro()

# ===
# Function to create libraries
# ===
function(db_lib)
    cmake_parse_arguments(DB_LIB

            "STATIC;SHARED;SKIP_API"
            "NAME"
            "SRCS;HEADERS;EXPORTS;DEFINES;PCH;"
            ${ARGN}
            )

    # set the target name accordingly
    if (DB_LIB_NAME)
        set(target_name ${DB_LIB_NAME})
    else()
        set(target_name ${PROJECT_NAME})
    endif()

    #
    # Variables used in the config file and setup lib variables
    #
    string(TOUPPER ${target_name} target_name_upper)
    set(generated_dir ${CMAKE_CURRENT_SOURCE_DIR})

    if (NOT DB_LIB_SKIP_API)
        # Congiure the targets API
        set(target_config_file ${target_name_upper}_API.h)
        configure_file("${DB_CMAKE_MODULE_PATH}/TargetAPI.h.in" "${generated_dir}/${target_config_file}")

        # Add the generated config header to the list of generated headers
        list(APPEND generated_headers
                "${generated_dir}/${target_config_file}"
                )
    endif()

    # Build both a static and shared library variant if the build types was not
    # specifically set
    if (NOT DB_LIB_STATIC AND NOT DB_LIB_SHARED)
        set(DB_LIB_STATIC TRUE)
        set(DB_LIB_SHARED TRUE)
    endif()

    # set the libraries configuration dir
    set(lib_config_dir ${CMAKE_PREFIX_PATH})

    set(all_headers "${generated_headers};${DB_LIB_HEADERS}")

    set(lib_install_include_dir "${DB_INCLUDE_PATH_NAME}/${target_name}")

    # install the headers to the install include path
    install(FILES ${all_headers} DESTINATION "${DB_INCLUDE_PATH}/${target_name}")

    add_library(${target_name}_Headers INTERFACE)

    target_compile_definitions(${target_name}_Headers INTERFACE cxx_std_${DB_CXX_VERSION})
    # standard compiler warnings
    set_project_warnings(${target_name}_Headers)
    # santitizer options if supported by compiler
    enable_sanitizers(${target_name}_Headers)
    if (DB_LIB_PCH)
        target_compile_headers(${target_name}_Headers INTERFACE ${DB_LIB_PCH})
    endif()
    db_set_compile_definitions(${target_name}_Headers)
    if (ENABLE_UNITY)
        set_target_properties(${target_name}_Headers UNITY_BUILD ON)
    endif()

    set(include_paths "${DB_INCLUDE_PATH}")
    target_include_directories(${target_name}_Headers INTERFACE
            "$<BUILD_INTERFACE:${include_paths}>"
            "$<INSTALL_INTERFACE:${DB_INCLUDE_PATH_NAME}>"
            )

    # Depending on the interface use a different path so that target_sources()
    # does not complain
    foreach(src ${DB_LIB_SRCS} ${all_headers})
        get_filename_component(real_path "${src}" ABSOLUTE)
        list(APPEND abs_source_files
                "$<BUILD_INTERFACE:${real_path}>"
                "$<INSTALL_INTERFACE:${lib_install_include_dir}/${src}>"
                )
    endforeach()

    cmake_policy(PUSH)
    cmake_policy(SET CMP0076 NEW)
    target_sources(${target_name}_Headers INTERFACE ${abs_source_files})
    cmake_policy(POP)

    if (DB_LIB_STATIC)
        set(target_lib_name ${target_name}_Static)

        # create the static library target
        add_library(${target_lib_name} STATIC
                ${DB_LIB_SRCS}
                )
        # Copy headers to install path include directory so that the build script
        # will know about any new headers that may have been added between installs
        copy_headers(${target_lib_name} "${all_headers}")

        target_link_libraries(${target_lib_name} PUBLIC ${target_name}_Headers)

        db_install(TRUE)

        set_target_properties(${target_lib_name}
                PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY    "${DB_LIB_INSTALL_PATH}"
                LIBRARY_OUTPUT_DIRECTORY    "${DB_LIB_INSTALL_PATH}"
                RUNTIME_OUTPUT_DIRECTORY    "${DB_BIN_INSTALL_PATH}"
                OUTPUT_NAME                 "${target_name}"
                PREFIX                      "lib"
                )

        target_compile_definitions(${target_lib_name}
                PRIVATE
                DB_IS_STATIC
                )
    endif()

    if (DB_LIB_SHARED)
        set(target_lib_name ${target_name})

        # create the shared library target
        add_library(${target_lib_name} SHARED
                ${DB_LIB_SRCS}
                )
        # Copy headers to install path include directory so that the build script
        # will know about any new headers that may have been added between installs
        copy_headers(${target_lib_name} "${all_headers}")

        target_link_libraries(${target_lib_name} PUBLIC ${target_name}_Headers)

        db_install(FALSE)

        list(APPEND all_targets ${target_lib_name})

        set_target_properties(${target_lib_name}
                PROPERTIES
                ARCHIVE_OUTPUT_DIRECTORY    "${DB_LIB_INSTALL_PATH}"
                LIBRARY_OUTPUT_DIRECTORY    "${DB_LIB_INSTALL_PATH}"
                RUNTIME_OUTPUT_DIRECTORY    "${DB_BIN_INSTALL_PATH}"
                OUTPUT_NAME                 "${target_name}"
                )

        target_compile_definitions(${target_lib_name}
                PRIVATE
                DB_EXPORT_DLL
                )
    endif()
endfunction()

# ===
# Helper function to link libraries
# ===

function( db_link_libraries )
    cmake_parse_arguments(DB_LIB
            "STATIC;SHARED"
            "NAME"
            "PUBLIC;PRIVATE:INTERFACE"
            ${ARGN}
            )

    if (DB_LIB_NAME)
        set(target_name ${DB_LIB_NAME})
    else()
        set(target_name ${PROJECT_NAME})
    endif()
    # Configure both if library type was not specified.
    if (NOT DB_LIB_STATIC AND NOT DB_LIB_SHARED)
        set(DB_LIB_STATIC TRUE)
        set(DB_LIB_SHARED TRUE)
    endif()

    set(args "")
    if (DB_LIB_PUBLIC)
        set(args ${args} PUBLIC ${DB_LIB_PUBLIC})
    endif()
    if (DB_LIB_PRIVATE)
        set(args ${args} PRIVATE ${DB_LIB_PRIVATE})
    endif()
    if (DB_LIB_INTERFACE)
        set(args ${args} INTERFACE ${DB_LIB_INTERFACE})
    endif()

    if (DB_LIB_STATIC AND TARGET "${target_name}_Static")
        target_link_libraries(${target_name}_Static ${args})
    endif()
    if (DB_LIB_SHARED AND TARGET "${target_name}")
        target_link_libraries(${target_name} ${args})
    endif()
endfunction()

# ===
# Helper function to create executables
# ===
function(db_exec)
    cmake_parse_arguments(DB_EXEC
        ""
        "NAME"
        "SRCS;HEADERS;PCH"
        ${ARGN}
    )

    if (${DB_EXEC_NAME})
        set(target_name ${DB_EXEC_NAME})
    else()
        set(target_name ${PROJECT_NAME})
    endif()

    add_executable(${target_name} ${DB_EXEC_SRCS} ${DB_EXEC_HEADERS})

    target_compile_definitions(${target_name} PRIVATE cxx_std_${DB_CXX_VERSION})

    # standard compiler warnings
    set_project_warnings(${target_name})
    # santitizer options if supported by compiler
    enable_sanitizers(${target_name})
    if (DB_EXEC_PCH)
        target_compile_headers(${target_name} PRIVATE ${DB_EXEC_PCH})
    endif()
    db_set_compile_definitions(${target_name})
    if (ENABLE_UNITY)
        set_target_properties(${target_name} UNITY_BUILD ON)
    endif()

    install(
        TARGETS ${target_name}
        RUNTIME DESTINATION ${DB_BIN_INSTALL_PATH_NAME}
    )
endfunction()

macro(db_copy_assets target)
    #file(COPY ${DB_ROOT_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets/)
    add_custom_command(
        TARGET ${target} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets "${CMAKE_BINARY_DIR}/assets"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT  "Copying assets..."
    )
endmacro()
