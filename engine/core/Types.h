// Conduit fundamental types and special-member helpers.

#ifndef CONDUIT_CORE_TYPES_H
#define CONDUIT_CORE_TYPES_H

#include <cstdint>

namespace Conduit
{

/// Signed integers whose names state their exact storage width.
using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

/// Unsigned integers whose names state their exact storage width.
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

} // namespace Conduit

/// Deletes copy construction and copy assignment for a type.
#define CONDUIT_NO_COPY(TypeName)        \
    TypeName(const TypeName&) = delete;  \
    TypeName& operator=(const TypeName&) = delete

/// Deletes move construction and move assignment for a type.
#define CONDUIT_NO_MOVE(TypeName)   \
    TypeName(TypeName&&) = delete;  \
    TypeName& operator=(TypeName&&) = delete

#endif // CONDUIT_CORE_TYPES_H
