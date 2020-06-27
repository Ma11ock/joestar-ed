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

/* Check the Lua state to make sure there are no errors. */
static bool check_lua(lua_State *ls, int r);

/* Ensure the lua type of the wrapper struct is the right type */
bool ensure_lua_type(jlua_var *var, jlua_type ltype)
{
    bool result = false;
    if(var->type == ltype)
    {
        result = true;
    }

    return result;
}

/* JLVar Constructor */
jlua_var jlvar_init(jlua_type type)
{
    struct jlua_var result =
    {
        .type = type
    };
    return result;
}

/* JLVar Dynamic Constructor */
jlua_var *jlvar_malloc(jlua_type type)
{
    struct jlua_var *newVar = joe_malloc(sizeof(struct jlua_var));
    *newVar = jlvar_init(type);

    return newVar;
}

/* JLVar dynamic Destructor */
void jlvar_free_and_nil(jlua_var **var)
{
    joe_free(*var);
    *var = NULL;
}

/* Init Lua */
void init_lua()
{
    L = luaL_newstate();
    luaL_openlibs(L);

    /* TODO for testing purposes we will simply load joesinit.lua. */
    if(check_lua(L, luaL_dofile(L, "joesinit.lua")))
    {
        lua_getglobal(L, "kek");
        if(lua_isnumber(L, -1))
            fprintf(stderr, "%d\n", (int)lua_tointeger(L, -1));
    }
    else
    {
        fprintf(stderr, "Error!\n");
    }
}

/* End Lua  */
void free_lua()
{
    lua_close(L);
    L = NULL;
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