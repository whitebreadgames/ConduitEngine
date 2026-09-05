#include "framework/Test.h"

#include <Conduit/core/Version.h>

#include <cstring>

using namespace Conduit;

CONDUIT_TEST(EngineVersionMatchesBuildConfiguration)
{
    const Version version = GetEngineVersion();

    CONDUIT_CHECK_EQ(version.major, static_cast<std::uint32_t>(CONDUIT_VERSION_MAJOR));
    CONDUIT_CHECK_EQ(version.minor, static_cast<std::uint32_t>(CONDUIT_VERSION_MINOR));
    CONDUIT_CHECK_EQ(version.patch, static_cast<std::uint32_t>(CONDUIT_VERSION_PATCH));
}

CONDUIT_TEST(EngineVersionStringIsPopulated)
{
    const char* text = GetEngineVersionString();

    CONDUIT_REQUIRE(text != nullptr);
    CONDUIT_CHECK(std::strlen(text) > 0);
    CONDUIT_CHECK(std::strchr(text, '.') != nullptr);
}

CONDUIT_TEST(VersionEncodingOrdersCorrectly)
{
    // Encoding must be monotonic across every component, so a packed comparison is a
    // valid stand-in for a field-by-field SemVer precedence check.
    CONDUIT_CHECK(CONDUIT_VERSION_ENCODE(0, 1, 0) < CONDUIT_VERSION_ENCODE(0, 2, 0));
    CONDUIT_CHECK(CONDUIT_VERSION_ENCODE(0, 9, 9) < CONDUIT_VERSION_ENCODE(1, 0, 0));
    CONDUIT_CHECK(CONDUIT_VERSION_ENCODE(1, 0, 1) > CONDUIT_VERSION_ENCODE(1, 0, 0));
    CONDUIT_CHECK(CONDUIT_VERSION_ENCODE(0, 0, 999) < CONDUIT_VERSION_ENCODE(0, 1, 0));
}

CONDUIT_TEST(FormatVersionsAreIndependentOfEngineVersion)
{
    // A user's project must survive engine upgrades. These only move when the bytes on
    // disk change, never because the engine version moved.
    CONDUIT_CHECK(kSceneFormatVersion >= kMinSupportedSceneFormatVersion);
    CONDUIT_CHECK(kProjectFormatVersion >= kMinSupportedProjectFormatVersion);
    CONDUIT_CHECK(kSceneFormatVersion > 0);
    CONDUIT_CHECK(kProjectFormatVersion > 0);
}

CONDUIT_TEST(ClassifyFormatVersionCoversEveryLoaderPath)
{
    // current = 3, oldest migratable = 2
    CONDUIT_CHECK(ClassifyFormatVersion(3, 3, 2) == FormatCompatibility::Current);
    CONDUIT_CHECK(ClassifyFormatVersion(2, 3, 2) == FormatCompatibility::Migratable);
    CONDUIT_CHECK(ClassifyFormatVersion(1, 3, 2) == FormatCompatibility::TooOld);
    CONDUIT_CHECK(ClassifyFormatVersion(4, 3, 2) == FormatCompatibility::TooNew);

    // A newer file is refused rather than partially read: silently dropping fields the
    // loader does not understand is how a project gets corrupted (AGENTS.md §6).
    CONDUIT_CHECK(ClassifyFormatVersion(99, 1, 1) == FormatCompatibility::TooNew);
}

CONDUIT_TEST(ClassifyFormatVersionHandlesSingleVersionFormats)
{
    // Where nothing has ever been migrated, current == minimum supported.
    CONDUIT_CHECK(ClassifyFormatVersion(1, 1, 1) == FormatCompatibility::Current);
    CONDUIT_CHECK(ClassifyFormatVersion(0, 1, 1) == FormatCompatibility::TooOld);
}
