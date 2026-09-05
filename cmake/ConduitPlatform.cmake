# Detects the target platform and exposes the result as cache-visible variables
# plus compile definitions on the Conduit::Platform interface target.
#
# This is the ONLY place platform detection happens. Per AGENTS.md §4 no platform
# #ifdef may appear above engine/platform/ — code that needs to branch on the OS
# belongs in a backend, not behind a macro in shared code.

set(CONDUIT_PLATFORM_WINDOWS OFF)
set(CONDUIT_PLATFORM_MACOS   OFF)
set(CONDUIT_PLATFORM_LINUX   OFF)
set(CONDUIT_PLATFORM_IOS     OFF)
set(CONDUIT_PLATFORM_ANDROID OFF)

if(ANDROID)
    set(CONDUIT_PLATFORM_ANDROID ON)
    set(CONDUIT_PLATFORM_NAME "Android")
elseif(IOS)
    set(CONDUIT_PLATFORM_IOS ON)
    set(CONDUIT_PLATFORM_NAME "iOS")
elseif(WIN32)
    set(CONDUIT_PLATFORM_WINDOWS ON)
    set(CONDUIT_PLATFORM_NAME "Windows")
elseif(APPLE)
    set(CONDUIT_PLATFORM_MACOS ON)
    set(CONDUIT_PLATFORM_NAME "macOS")
elseif(UNIX)
    set(CONDUIT_PLATFORM_LINUX ON)
    set(CONDUIT_PLATFORM_NAME "Linux")
else()
    message(FATAL_ERROR "Conduit: unrecognised target platform '${CMAKE_SYSTEM_NAME}'.")
endif()

# Mobile targets are touch-first and drive the adaptive editor UX (TODO Phase 9).
if(CONDUIT_PLATFORM_IOS OR CONDUIT_PLATFORM_ANDROID)
    set(CONDUIT_PLATFORM_MOBILE ON)
else()
    set(CONDUIT_PLATFORM_MOBILE OFF)
endif()

add_library(ConduitPlatformDefs INTERFACE)
add_library(Conduit::Platform ALIAS ConduitPlatformDefs)

target_compile_definitions(ConduitPlatformDefs INTERFACE
    CONDUIT_PLATFORM_WINDOWS=$<BOOL:${CONDUIT_PLATFORM_WINDOWS}>
    CONDUIT_PLATFORM_MACOS=$<BOOL:${CONDUIT_PLATFORM_MACOS}>
    CONDUIT_PLATFORM_LINUX=$<BOOL:${CONDUIT_PLATFORM_LINUX}>
    CONDUIT_PLATFORM_IOS=$<BOOL:${CONDUIT_PLATFORM_IOS}>
    CONDUIT_PLATFORM_ANDROID=$<BOOL:${CONDUIT_PLATFORM_ANDROID}>
    CONDUIT_PLATFORM_MOBILE=$<BOOL:${CONDUIT_PLATFORM_MOBILE}>
    $<$<CONFIG:Debug>:CONDUIT_DEBUG=1>
    $<$<NOT:$<CONFIG:Debug>>:CONDUIT_DEBUG=0>
)

message(STATUS "Conduit: target platform is ${CONDUIT_PLATFORM_NAME}")
