#include "jlua.hpp"
#include "bridge.hpp"
#include <iostream>


static lua_State *L = nullptr;

///////////////////////////////////////////////////////////////////////////////
//                                libjoestar                                 //
///////////////////////////////////////////////////////////////////////////////

/* Function that exists solely for debugging purposes */
static int testf(lua_State *l)
{
    std::cerr << "testf called\n";
    return 0;
}

static int get_from_joestar(lua_State *l)
{
    
}

/* Take change in lua state and alter joe internal variables accordingly.
   Returns false on error and true on success. */
static int sync_to_joestar(lua_State *l)
{
    std::string_view name = lua_tostring(l, 1);
    if(lua_isinteger(l, 2))
    {
        if(!jlua::setVal(name, static_cast<int>(lua_tointeger(l, 2))))
        {
            std::cerr << "It was not updated correctly.\n";
            lua_pushboolean(l, false);
            return 1;
        }
    }
    else if(lua_isboolean(l, 2))
    {
        if(!jlua::setVal(name, static_cast<bool>(lua_toboolean(l, 2))))
        {
            /* TODO */
            lua_pushboolean(l, false);
            return 1;
        }
    }
    else if(lua_isstring(l, 2))
    {
        if(!jlua::setVal(name, lua_tostring(l, 2)))
        {
            /* TODO */
            lua_pushboolean(l, false);
            return 1;
        }
    }

    lua_pushboolean(l, true);
    return 1;
}

static const luaL_Reg libjoestar[] =
{
    { "jsync_l", sync_to_joestar },
    { "testf", testf },
};

///////////////////////////////////////////////////////////////////////////////
//                                public funcs                               //
///////////////////////////////////////////////////////////////////////////////


bool jlua::do_file(std::filesystem::path path)
{
    if(luaL_dofile(L, path.c_str()) != LUA_OK)
    {
        std::cerr << "Lua failed: " << lua_tostring(L, -1) << '\n';
        lua_pop(L, 1);
        return false;
        /*TODO failstate*/
    }
    std::cerr << "Loaded file " << path << '\n';
    return true;
}

void jlua::init()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    std::size_t libjstarSize = sizeof(libjoestar) / sizeof(luaL_Reg);
    lua_createtable(L, 0, 2);
    for(auto &libfunc : libjoestar)
    {
        lua_pushcfunction(L, libfunc.func);
        lua_setfield(L, -2, libfunc.name);
    }
    lua_setglobal(L, "joestar_internal");

    do_file("/home/ryan/src/joestar/src/init.lua");
    do_file("/home/ryan/src/joestar/src/test.lua");
}

void jlua::free()
{
    lua_close(L);
    L = nullptr;
}