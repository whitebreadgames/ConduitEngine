#include "framework/Test.h"

#include <engine/core/Types.h>

#include <cstdint>
#include <type_traits>

using namespace Conduit;

namespace
{

struct NonCopyable
{
    NonCopyable() = default;
    CONDUIT_NO_COPY(NonCopyable);
};

struct NonMovable
{
    NonMovable() = default;
    CONDUIT_NO_MOVE(NonMovable);
};

static_assert(!std::is_copy_constructible<NonCopyable>::value);
static_assert(!std::is_copy_assignable<NonCopyable>::value);
static_assert(!std::is_move_constructible<NonMovable>::value);
static_assert(!std::is_move_assignable<NonMovable>::value);

} // namespace

CONDUIT_TEST(FundamentalIntegerAliasesHaveExactWidths)
{
    CONDUIT_CHECK_EQ(sizeof(Int8), static_cast<std::size_t>(1));
    CONDUIT_CHECK_EQ(sizeof(Int16), static_cast<std::size_t>(2));
    CONDUIT_CHECK_EQ(sizeof(Int32), static_cast<std::size_t>(4));
    CONDUIT_CHECK_EQ(sizeof(Int64), static_cast<std::size_t>(8));
    CONDUIT_CHECK_EQ(sizeof(UInt8), static_cast<std::size_t>(1));
    CONDUIT_CHECK_EQ(sizeof(UInt16), static_cast<std::size_t>(2));
    CONDUIT_CHECK_EQ(sizeof(UInt32), static_cast<std::size_t>(4));
    CONDUIT_CHECK_EQ(sizeof(UInt64), static_cast<std::size_t>(8));
}

CONDUIT_TEST(FundamentalIntegerAliasesPreserveSignedness)
{
    CONDUIT_CHECK(std::is_signed<Int8>::value);
    CONDUIT_CHECK(std::is_signed<Int16>::value);
    CONDUIT_CHECK(std::is_signed<Int32>::value);
    CONDUIT_CHECK(std::is_signed<Int64>::value);
    CONDUIT_CHECK(std::is_unsigned<UInt8>::value);
    CONDUIT_CHECK(std::is_unsigned<UInt16>::value);
    CONDUIT_CHECK(std::is_unsigned<UInt32>::value);
    CONDUIT_CHECK(std::is_unsigned<UInt64>::value);
}
