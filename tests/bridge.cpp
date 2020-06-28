#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

extern "C"
{
#include "../src/bridge.h"
}



size_t genRandomSize(size_t minn = 1, size_t maxx = 20)
{
    using sysClock = std::chrono::system_clock;
    std::mt19937 randomGenerator((long unsigned int)sysClock::now().time_since_epoch().count());
    std::uniform_int_distribution<size_t> dis(minn, maxx);
    return dis(randomGenerator);
}

std::string gen_random(size_t len)
{
    std::string s(len, 'c');
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i < len; ++i)
    {
        s[i] = alphanum[genRandomSize() % (sizeof(alphanum) - 1)];
    }
    return s;
}

// Test the quality of the hash function
TEST(bridge, hash)
{
    const int tests = 500;
    int collisions = 0;
    std::vector<unsigned long> results;
    for(int i = 0; i < tests; i++)
    {
        size_t s = genRandomSize();
        std::string testSubject = gen_random(s);
        results.push_back(var_hash(testSubject.c_str()));
    }

    for(size_t i = 0; i < HASH_MAX; i++)
    {
        bool wasCollide = false;
        for(size_t j = 0; j < results.size(); j++)
        {
            if(results[j] == i)
            {
                collisions++;
                wasCollide = true;
            }
        }
        if(wasCollide)
            collisions--;
    }

    ASSERT_LT((double)tests / (double)collisions, 20.0); // assert that no more than 20% of the
                                                         // hashes collide
}