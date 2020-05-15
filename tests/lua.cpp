#include <gtest/gtest.h>

extern "C"
{
#include "../src/jlua.h"
}

TEST(lua, ensure_type_true)
{
    jlua_var var = jlvar_init(LUA_NIL);

    ASSERT_EQ(ensure_lua_type(&var, LUA_NIL), JTRUE);
}

TEST(lua, ensure_type_false)
{
    jlua_var var = jlvar_init(LUA_NIL);

    ASSERT_EQ(ensure_lua_type(&var, LUA_REAL), JFALSE);
}

TEST(lua, destructor)
{
    jlua_var *testVar = jlvar_malloc(LUA_REAL);

    jlvar_free_and_nil(&testVar);
    ASSERT_EQ(testVar, nullptr);
}