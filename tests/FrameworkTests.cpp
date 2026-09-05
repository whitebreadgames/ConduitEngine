#include "framework/Test.h"

#include <string>

// The harness has to be trustworthy before any result it reports means anything.

CONDUIT_TEST(FrameworkReportsPassingChecks)
{
    CONDUIT_CHECK(true);
    CONDUIT_CHECK_EQ(2 + 2, 4);
    CONDUIT_CHECK_EQ(std::string("conduit"), std::string("conduit"));
}

CONDUIT_TEST(FrameworkDescribesStreamableValues)
{
    CONDUIT_CHECK_EQ(Conduit::Testing::Describe(42), std::string("42"));
    CONDUIT_CHECK_EQ(Conduit::Testing::Describe(std::string("node")), std::string("node"));
}

CONDUIT_TEST(FrameworkDescribesNonStreamableValues)
{
    struct Opaque
    {
        int value;
    };

    CONDUIT_CHECK_EQ(Conduit::Testing::Describe(Opaque{7}),
                     std::string("<non-printable value>"));
}
