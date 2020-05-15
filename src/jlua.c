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
static jbool check_lua(int r);

/* Ensure the lua type of the wrapper struct is the right type */
jbool ensure_lua_type(jlua_var *var, jlua_type ltype)
{
    jbool result = JFALSE;
    if(var->type == ltype)
    {
        result = JTRUE;
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
}

/* End Lua  */
void free_lua()
{
    lua_close(L);
    L = NULL;
}

/* Check the Lua state to make sure there are no errors. */
static jbool check_lua(int r)
{
    jbool result = JTRUE;
    if(r != LUA_OK)
    {
        //= lua_tostring(L, -1); TODO handle error
        result = JFALSE;
    }

    return result;
}