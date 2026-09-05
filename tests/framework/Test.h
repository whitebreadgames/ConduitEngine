// Conduit minimal test framework.
//
// Original MIT code, deliberately dependency-free. Conduit vendors nothing it can
// write in a hundred lines: a test harness that is part of the build has no version
// skew, no license question, and compiles on every console toolchain we will meet.
// If the suite ever outgrows this, swapping in a vendored framework is a contained
// change because tests only touch the four macros below.
//
// Usage:
//     CONDUIT_TEST(HandleRejectsStaleGeneration)
//     {
//         CONDUIT_CHECK(handle.IsValid());
//         CONDUIT_CHECK_EQ(table.Resolve(stale), nullptr);
//     }
//
// Tests self-register at static-initialisation time; main() lives in Main.cpp.

#ifndef CONDUIT_TESTS_TEST_H
#define CONDUIT_TESTS_TEST_H

#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Conduit::Testing
{

using TestFunction = void (*)();

struct TestCase
{
    const char* name;
    const char* file;
    int line;
    TestFunction function;
};

/// Registry of every test in the binary. Function-local static so registration
/// order across translation units is well defined.
inline std::vector<TestCase>& Registry()
{
    static std::vector<TestCase> registry;
    return registry;
}

/// Failures recorded by the currently running test.
inline std::vector<std::string>& CurrentFailures()
{
    static std::vector<std::string> failures;
    return failures;
}

struct Registrar
{
    Registrar(const char* name, const char* file, int line, TestFunction function)
    {
        Registry().push_back(TestCase{name, file, line, function});
    }
};

inline void RecordFailure(const char* file, int line, const std::string& message)
{
    std::ostringstream out;
    out << file << ":" << line << ": " << message;
    CurrentFailures().push_back(out.str());
}

/// Detects whether a type can be written to an ostream. Written as a C++17 trait
/// rather than a requires-clause because C++17 is the baseline (AGENTS.md §4).
template <typename T, typename = void>
struct IsStreamable : std::false_type
{
};

template <typename T>
struct IsStreamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T&>())>>
    : std::true_type
{
};

/// Streams a value into a failure message, falling back to a placeholder for types
/// that have no operator<<.
template <typename T>
std::enable_if_t<IsStreamable<T>::value, std::string> Describe(const T& value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

template <typename T>
std::enable_if_t<!IsStreamable<T>::value, std::string> Describe(const T&)
{
    return "<non-printable value>";
}

int RunAll(const char* filter);

} // namespace Conduit::Testing

#define CONDUIT_TEST_CONCAT_INNER(a, b) a##b
#define CONDUIT_TEST_CONCAT(a, b) CONDUIT_TEST_CONCAT_INNER(a, b)

/// Defines and registers a test case.
#define CONDUIT_TEST(name)                                                        \
    static void CONDUIT_TEST_CONCAT(ConduitTest_, name)();                        \
    static const ::Conduit::Testing::Registrar CONDUIT_TEST_CONCAT(               \
        ConduitTestRegistrar_, name){#name, __FILE__, __LINE__,                   \
                                     &CONDUIT_TEST_CONCAT(ConduitTest_, name)};   \
    static void CONDUIT_TEST_CONCAT(ConduitTest_, name)()

/// Records a failure and continues, so one test can report several problems.
#define CONDUIT_CHECK(expr)                                                       \
    do                                                                            \
    {                                                                             \
        if (!(expr))                                                              \
        {                                                                         \
            ::Conduit::Testing::RecordFailure(__FILE__, __LINE__,                 \
                                              "expected: " #expr);                \
        }                                                                         \
    } while (false)

#define CONDUIT_CHECK_EQ(actual, expected)                                        \
    do                                                                            \
    {                                                                             \
        const auto& conduitActual = (actual);                                     \
        const auto& conduitExpected = (expected);                                 \
        if (!(conduitActual == conduitExpected))                                  \
        {                                                                         \
            ::Conduit::Testing::RecordFailure(                                    \
                __FILE__, __LINE__,                                               \
                std::string("expected: " #actual " == " #expected "\n    actual: ") \
                    + ::Conduit::Testing::Describe(conduitActual)                 \
                    + "\n  expected: "                                            \
                    + ::Conduit::Testing::Describe(conduitExpected));             \
        }                                                                         \
    } while (false)

/// Records a failure and abandons the rest of the test body.
#define CONDUIT_REQUIRE(expr)                                                     \
    do                                                                            \
    {                                                                             \
        if (!(expr))                                                              \
        {                                                                         \
            ::Conduit::Testing::RecordFailure(__FILE__, __LINE__,                 \
                                              "required: " #expr);                \
            return;                                                               \
        }                                                                         \
    } while (false)

#endif // CONDUIT_TESTS_TEST_H
