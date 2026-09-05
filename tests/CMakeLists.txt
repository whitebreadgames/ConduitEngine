# ConduitTests — the test suite.

add_executable(ConduitTests
    framework/TestMain.cpp
    FrameworkTests.cpp
    TypesTests.cpp
    VersionTests.cpp
)

target_include_directories(ConduitTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(ConduitTests
    PRIVATE Conduit::Core Conduit::Warnings
)

set_target_properties(ConduitTests PROPERTIES OUTPUT_NAME "conduit-tests")

add_test(NAME ConduitTests COMMAND ConduitTests)
