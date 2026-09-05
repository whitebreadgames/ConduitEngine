#include <Conduit/core/Version.h>

namespace Conduit
{

// The generated header is the single source of truth for these numbers. Assert the
// invariants here so a bad edit to the template fails the build rather than shipping
// a binary that disagrees with the files it writes.
static_assert(kSceneFormatVersion >= kMinSupportedSceneFormatVersion,
              "Scene format version cannot be older than the oldest version supported.");
static_assert(kProjectFormatVersion >= kMinSupportedProjectFormatVersion,
              "Project format version cannot be older than the oldest version supported.");

static_assert(ClassifyFormatVersion(2, 1, 1) == FormatCompatibility::TooNew, "");
static_assert(ClassifyFormatVersion(1, 1, 1) == FormatCompatibility::Current, "");
static_assert(ClassifyFormatVersion(2, 3, 2) == FormatCompatibility::Migratable, "");
static_assert(ClassifyFormatVersion(1, 3, 2) == FormatCompatibility::TooOld, "");

const char* GetEngineVersionString() noexcept
{
    return kEngineVersionString;
}

Version GetEngineVersion() noexcept
{
    return kEngineVersion;
}

} // namespace Conduit
