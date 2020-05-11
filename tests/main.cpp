#include <gtest/gtest.h>
#include "../src/types.h"

int main(int argc, char **argv, char **env)
{
    (void)env;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
