cmake_minimum_required(VERSION 3.20)

# Conduit — a lightweight, high-performance 2D game engine and cross-platform editor.
# MIT licensed. See LICENSE. Contributor contract: AGENTS.md.
project(Conduit
    VERSION 0.1.0
    DESCRIPTION "Lightweight 2D game engine and cross-platform editor"
    HOMEPAGE_URL "https://github.com/whitebreadgames/ConduitEngine"
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR
        "Conduit requires an out-of-source build.\n"
        "Run: cmake -B build && cmake --build build")
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# --- Options ----------------------------------------------------------------

option(CONDUIT_BUILD_EDITOR      "Build the Conduit editor"            ON)
option(CONDUIT_BUILD_RUNTIME     "Build the standalone game runtime"   ON)
option(CONDUIT_BUILD_TESTS       "Build the test suite"                ON)
option(CONDUIT_WARNINGS_AS_ERRORS "Treat compiler warnings as errors"  OFF)

# --- Language standard ------------------------------------------------------
#
# AGENTS.md §4: C++17 is the baseline, C++20 where every target toolchain
# supports it. The baseline is what ships; opt into 20 deliberately, not by
# accident, so a C++20-only construct cannot silently break a console port.

set(CONDUIT_CXX_STANDARD 17 CACHE STRING "C++ standard for Conduit targets (17 or 20)")
set_property(CACHE CONDUIT_CXX_STANDARD PROPERTY STRINGS 17 20)

set(CMAKE_CXX_STANDARD ${CONDUIT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # no compiler-specific extensions in portable code

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # for clangd / IDE tooling

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# --- Shared configuration targets -------------------------------------------

include(ConduitWarnings)
include(ConduitPlatform)

# --- Subprojects ------------------------------------------------------------
#
# Dependency arrow points one way: editor -> engine, runtime -> engine.
# The engine never includes an editor header (AGENTS.md §4).

add_subdirectory(engine)

if(CONDUIT_BUILD_RUNTIME)
    add_subdirectory(runtime)
endif()

if(CONDUIT_BUILD_EDITOR)
    add_subdirectory(editor)
endif()

if(CONDUIT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# --- Summary ----------------------------------------------------------------

message(STATUS "Conduit ${PROJECT_VERSION}")
message(STATUS "  C++ standard      : ${CONDUIT_CXX_STANDARD}")
message(STATUS "  Build type        : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Editor            : ${CONDUIT_BUILD_EDITOR}")
message(STATUS "  Runtime           : ${CONDUIT_BUILD_RUNTIME}")
message(STATUS "  Tests             : ${CONDUIT_BUILD_TESTS}")
message(STATUS "  Warnings as errors: ${CONDUIT_WARNINGS_AS_ERRORS}")
