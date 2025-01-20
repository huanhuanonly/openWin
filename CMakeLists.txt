# ----------------------------------------------------------------------------
# Copyright (c) 2024-2025 Yang Huanhuan (3347484963@qq.com).
# 
# This software is provided "as is", without warranty of any kind, express or implied.
# 
# 
# Root CMake file for openWin (https://github.com/huanhuanonly/openWin)
# 
# Created by Yang Huanhuan on December 31, 2024, 21:55:51
# ----------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.8)
project (openWin VERSION 0.1.0 LANGUAGES CXX)

include (FetchContent)

if (POLICY CMP0141)
    cmake_policy (SET CMP0141 NEW)
    set (CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message (STATUS "<DEBUG MODE> ---")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    message (STATUS "<RELEASE MODE> ---")
else()
    message (STATUS "<OTHER MODE> ---")
endif()

file (GLOB SOURCE_FILE CONFIGURE_DEPENDS "src/*.cpp")

set (OPENWIN_TOOL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/openWin/tools")

# Download and import cpp-kwargs
set (CPPKWARGS_REPOS "https://github.com/huanhuanonly/cpp-kwargs.git")
set (CPPKWARGS_PATH "${OPENWIN_TOOL_PATH}/cpp-kwargs")

if (NOT EXISTS ${CPPKWARGS_PATH})
	FetchContent_Declare (
        CppKwargs
        GIT_REPOSITORY ${CPPKWARGS_REPOS}
        GIT_TAG main
        GIT_SHALLOW TRUE
        SOURCE_DIR ${CPPKWARGS_PATH}
    )

    FetchContent_MakeAvailable (CppKwargs)
endif()


message (STATUS "CMAKE_CURRENT_SOURCE_DIR is: ${CMAKE_CURRENT_SOURCE_DIR}")
message (STATUS "CMAKE_CURRENT_BINARY_DIR is: ${CMAKE_CURRENT_BINARY_DIR}")

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories ("${CPPKWARGS_PATH}")

# Generate static library
add_library (openWin STATIC ${SOURCE_FILE})

if (CMAKE_BUILD_TYPE STREQUAL "Release")

    set_target_properties (openWin PROPERTIES
        OUTPUT_NAME "openWin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

endif()

target_compile_definitions (openWin PUBLIC KWARGSKEY_CASE_INSENSITIVE)

set_property (TARGET openWin PROPERTY CXX_STANDARD 20)

if (True)
    add_subdirectory ("${CMAKE_CURRENT_SOURCE_DIR}/test")
endif()