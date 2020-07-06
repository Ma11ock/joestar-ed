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

double get_global_float(const char *name)
{
    double result;
    lua_getglobal(L, name);
    result = lua_tonumber(L, 1);
    lua_pop(L, 1);
    return result;
}

void joe_set(const char *name, const char *value, jlua_type ltype)
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
}

/* End Lua  */
void free_lua()
{
    lua_close(L);
    L = NULL;
    free_vars();
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