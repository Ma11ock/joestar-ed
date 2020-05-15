#include <gtest/gtest.h>

extern "C"
{
#include "../src/lua.h"
}

TEST(lua, ensure_type_true)
{
    joes_var var = jvar_init(LUA_NIL);

    ASSERT_EQ(ensure_lua_type(&var, LUA_NIL), JTRUE);
}

TEST(lua, ensure_type_false)
{
    joes_var var = jvar_init(LUA_NIL);

    ASSERT_EQ(ensure_lua_type(&var, LUA_REAL), JFALSE);
}
