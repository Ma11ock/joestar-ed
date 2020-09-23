#include "jlua.hpp"
//#include "bridge.hpp"

static lua_State *L = nullptr;

/* Function that exists solely for debugging purposes */
static int testf(lua_State *l)
{
    return 0;
}

static int sync_to_joestar(lua_State *l)
{
    return 0;
}

static const luaL_Reg libjoestar[] =
{
    { "jsync_l", sync_to_joestar },
    { "testf", testf },
};

void jlua::init()
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

void jlua::free()
{
    lua_close(L);
    L = nullptr;
}