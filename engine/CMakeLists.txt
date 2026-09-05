# ConduitCore — the engine library. Knows nothing about the editor.

set(CONDUIT_VERSION_PRERELEASE "dev")

if(CONDUIT_VERSION_PRERELEASE STREQUAL "")
    set(CONDUIT_VERSION_STRING "${PROJECT_VERSION}")
else()
    set(CONDUIT_VERSION_STRING "${PROJECT_VERSION}-${CONDUIT_VERSION_PRERELEASE}")
endif()

set(CONDUIT_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/core/Version.h.in"
    "${CONDUIT_GENERATED_DIR}/Conduit/core/Version.h"
    @ONLY
)

add_library(ConduitCore STATIC
    core/Version.cpp
)
add_library(Conduit::Core ALIAS ConduitCore)

target_include_directories(ConduitCore
    PUBLIC
        "${PROJECT_SOURCE_DIR}"      # includes are rooted at the repo: <engine/core/...>
        "${CONDUIT_GENERATED_DIR}"   # generated headers: <Conduit/core/Version.h>
)

target_link_libraries(ConduitCore
    PUBLIC  Conduit::Platform
    PRIVATE Conduit::Warnings
)

set_target_properties(ConduitCore PROPERTIES
    VERSION   "${PROJECT_VERSION}"
    POSITION_INDEPENDENT_CODE ON   # required for the Android/iOS shared-library targets
)
