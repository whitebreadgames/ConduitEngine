#include "framework/Test.h"

#include <cstring>
#include <string>

namespace Conduit::Testing
{

int RunAll(const char* filter)
{
    const std::vector<TestCase>& tests = Registry();

    std::size_t passed = 0;
    std::size_t failed = 0;
    std::size_t skipped = 0;

    for (const TestCase& test : tests)
    {
        if (filter != nullptr && std::strstr(test.name, filter) == nullptr)
        {
            ++skipped;
            continue;
        }

        CurrentFailures().clear();
        test.function();

        if (CurrentFailures().empty())
        {
            ++passed;
            std::cout << "[ PASS ] " << test.name << "\n";
        }
        else
        {
            ++failed;
            std::cout << "[ FAIL ] " << test.name << "\n";
            for (const std::string& failure : CurrentFailures())
            {
                std::cout << "         " << failure << "\n";
            }
        }
    }

    std::cout << "\n" << passed << " passed, " << failed << " failed";
    if (skipped > 0)
    {
        std::cout << ", " << skipped << " filtered out";
    }
    std::cout << std::endl;

    return failed == 0 ? 0 : 1;
}

} // namespace Conduit::Testing

int main(int argc, char** argv)
{
    // Optional substring filter: conduit-tests Version
    const char* filter = (argc > 1) ? argv[1] : nullptr;
    return Conduit::Testing::RunAll(filter);
}
