#include <gtest/gtest.h>
extern "C"
{
#include "../src/types.h"
#include "../src/jlua.h"
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
    init_lua();
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    free_lua();
    return result;
}
