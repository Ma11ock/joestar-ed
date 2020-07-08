/*
 *  Basic Lua interface utils.
 *
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of joestar.
 */

#include "types.h"
#include "jlua.h"
#include "bridge.h"

static lua_State *L = NULL;


double get_global_float(const char *name)
{
    double result;
    lua_getglobal(L, name);
    result = lua_tonumber(L, 1);
    lua_pop(L, 1);
    return result;
}

/* Set joestar variable value */
void joe_set(const char *name, const void *value, jlua_type ltype)
{
    struct joe_var newVar;
}

/*****************************************************************************/
/*                               Joestar Lua Interface                       */
/*****************************************************************************/

int l_set(lua_State *L)
{
    const char *s = lua_tostring(L, 1);
    if(lua_isstring(L, 1))
    {
        if(lua_isstring(L, 2))
        {
            fputs(lua_tostring(L, 2), stderr);
        }
    }
    else
    {
        /*TODO error*/
    }
    fputs(s, stderr);
    return 0;
}

static const struct luaL_Reg libjoestar [] =
{
    { "joeset", l_set },
};

static struct joe_var *joestar_var_names[] =
{
    &usermail,
    &username,
};

/*****************************************************************************/
/*****************************************************************************/


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
    for(size_t i = 0; i < sizeof(libjoestar) / sizeof(struct luaL_Reg); i++)
    {
        luaL_Reg tmp = libjoestar[i];
        lua_pushcfunction(L, tmp.func);
        lua_setglobal(L, tmp.name);
    }
    /* TODO for testing purposes we will simply load joesinit.lua. */
    joes_init_bridge(); /* initialize the joestar builtins */
    run_lua_script("./test.lua");
    /* Ask lua for values to variables */
    for(size_t i = 0; i < sizeof(joestar_var_names) / sizeof(struct joe_var*); i++)
    {
        struct joe_var *cur = joestar_var_names[i];
        lua_getglobal(L, cur->name);
        if(!lua_isnil(L, -1))
        {
            switch(cur->type)
            {
            case LUA_STRING:
                if(!lua_isstring(L, -1))
                {
                    /*error*/
                }
                else
                {
                }
                break;
            default:
                break;
            }
        }
        /* Variable is nil */
        else
        {
            joes_var_unset(cur->name);
        }
        lua_pop(L, 1);
    }
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

void jlua_set_string(const char *var_name, const char *str)
{
    if(str == NULL || var_name == NULL) /* output error msg? */
        return;

    lua_pushstring(L, var_name);
    lua_setglobal(L, str);
}