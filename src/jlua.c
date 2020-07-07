/*
 *  Basic Lua interface utils.
 *
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of joestar.
 */

#include "jlua.h"

#include "types.h"

static lua_State *L = NULL;
#include "bridge.h"

double get_global_float(const char *name)
{
    double result;
    lua_getglobal(L, name);
    result = lua_tonumber(L, 1);
    lua_pop(L, 1);
    return result;
}

/* Set joestar variable value */
void joe_set(const char *name, const char *value, jlua_type ltype)
{

}

/* Toggle joestar boolean variable */
void joe_toggle(const char *name)
{

}

void run_lua_script(const char *filepath)
{
    if(luaL_dofile(L, filepath) != LUA_OK)
    {
        goto lua_fail;
    }

    lua_pushglobaltable(L);
    lua_pushnil(L);

    while(lua_next(L, -2) != 0) /* Pop NIL, push name, value */
    {

        lua_pop(L, 1);
    }
    lua_pop(L, 1); /* Remove global table */
    fprintf(stderr, "Lua end\n");
	return;

lua_fail:
    /* TODO failstate */
    fprintf(stderr, "Not cool.\n");
}

/* Init Lua */
void init_lua()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    /* TODO for testing purposes we will simply load joesinit.lua. */
    run_lua_script("joesinit.lua");
    joes_init_bridge(); /* initialize the joestar builtins */
}

/* End Lua  */
void free_lua()
{
    lua_close(L);
    L = NULL;
    joes_free_vars(); /* destroy joestar builtins */
}

/* Check the Lua state to make sure there are no errors. */
static bool check_lua(lua_State *ls, int r)
{
    bool result = true;
    if(r != LUA_OK)
    {
        //= lua_tostring(L, -1); TODO handle error
        result = false;
    }

    return result;
}