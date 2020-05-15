#include <gtest/gtest.h>
extern "C"
{
#include "../src/types.h"
}

TEST(myTest, mtest)
{
    void *kek = joe_malloc(4l);
    ASSERT_NE(kek, (void*)NULL);
    joe_free(kek);
}

int main(int argc, char **argv, char **env)
{
    (void)env;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
