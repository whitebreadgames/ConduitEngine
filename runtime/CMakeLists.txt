# ConduitRuntime — the standalone game player.

add_executable(ConduitRuntime
    Main.cpp
)

target_link_libraries(ConduitRuntime
    PRIVATE Conduit::Core Conduit::Warnings
)

set_target_properties(ConduitRuntime PROPERTIES OUTPUT_NAME "conduit")
