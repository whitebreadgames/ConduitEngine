// Conduit runtime — standalone game player.
//
// Placeholder entry point. The real loop arrives with TODO 2.6 (fixed-timestep
// update, variable render, frame arena reset) once the platform layer exists.

#include <Conduit/core/Version.h>

#include <cstdio>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::printf("Conduit runtime %s\n", Conduit::GetEngineVersionString());
    std::printf("  scene format   : %u\n", Conduit::kSceneFormatVersion);
    std::printf("  project format : %u\n", Conduit::kProjectFormatVersion);
    std::printf("No project loaded. The runtime loop lands with TODO 2.6.\n");

    return 0;
}
