// Conduit editor — entry point.
//
// Placeholder shell. The windowed editor arrives with TODO 8.2, once the platform
// layer (Phase 2) and the UI integration (TODO 8.1) are in place. Deciding between
// Dear ImGui and RmlUi is open decision D2 in TODO.md.

#include <Conduit/core/Version.h>

#include <cstdio>

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    std::printf("Conduit editor %s\n", Conduit::GetEngineVersionString());
    std::printf("API is unstable until 1.0.0: %s\n", Conduit::kApiUnstable ? "yes" : "no");
    std::printf("Editor shell lands with TODO 8.2 (blocked on open decisions D1, D2).\n");

    return 0;
}
