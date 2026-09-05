# ConduitEditor — the cross-platform editor.
#
# Depends on the engine; the engine never depends back (AGENTS.md §4).

add_executable(ConduitEditor
    app/Main.cpp
)

target_link_libraries(ConduitEditor
    PRIVATE Conduit::Core Conduit::Warnings
)

set_target_properties(ConduitEditor PROPERTIES OUTPUT_NAME "conduit-editor")
