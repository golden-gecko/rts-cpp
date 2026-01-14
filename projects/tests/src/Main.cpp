#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "resources.*";

    int result = RUN_ALL_TESTS();

    std::cin.get();

    return result;
}
