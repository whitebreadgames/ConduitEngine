# Defines the Conduit::Warnings interface target.
#
# AGENTS.md §4 requires -Wall -Wextra -Wpedantic (/W4 on MSVC) with warnings as
# errors in CI. Locally warnings stay warnings so a work-in-progress build is not
# blocked; CI sets CONDUIT_WARNINGS_AS_ERRORS=ON to enforce the rule.

add_library(ConduitWarnings INTERFACE)
add_library(Conduit::Warnings ALIAS ConduitWarnings)

if(MSVC)
    target_compile_options(ConduitWarnings INTERFACE
        /W4
        /permissive-        # conforming mode; rejects MSVC-only extensions
        /Zc:__cplusplus     # report the real __cplusplus value
        /utf-8
    )
    if(CONDUIT_WARNINGS_AS_ERRORS)
        target_compile_options(ConduitWarnings INTERFACE /WX)
    endif()
else()
    target_compile_options(ConduitWarnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow                # a shadowed member in a frame loop is a silent bug
        -Wnon-virtual-dtor      # deleting through an interface pointer must be safe
        -Wold-style-cast
        -Wcast-align            # matters on the ARM targets (iOS, Android, Switch)
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wdouble-promotion      # an accidental double in the hot path costs real time
        -Wformat=2
    )
    if(CONDUIT_WARNINGS_AS_ERRORS)
        target_compile_options(ConduitWarnings INTERFACE -Werror)
    endif()
endif()
