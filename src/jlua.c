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

static bool types_match(jlua_type type, lua_State *l, int index)
{
    bool result = false;
    switch(type)
    {
    case LUA_STRING:
        result = lua_isstring(l, index);
        break;
    case LUA_REAL:
        result = lua_isnumber(l, index);
        break;
    case LUA_BOOL:
        result = lua_isboolean(l, index);
        break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
/*                               Joestar Lua Interface                       */
/*****************************************************************************/

static int l_sync_to_joestar(lua_State *l)
{
    struct joe_var *tmp = joes_get_var_by_name(lua_tostring(l, 1));

    if(tmp == NULL)
        return 0;

    if(lua_isnil(l, 2))
    {
        joes_var_unset_ref(tmp);
        return 0;
    }

    if(!types_match(tmp->type, l, 2))
    {
        /*TODO error*/
        fprintf(stderr, "Error!: %s and %s do not match\n", tmp->name, lua_tostring(l, 2));
        return 0;
    }

    switch(tmp->type)
    {
    case LUA_STRING:
        joes_set_var_string_ref(tmp, lua_tostring(l, 2));
        break;
    case LUA_REAL:
        joes_set_var_real_ref(tmp, lua_tonumber(l, 2));
        break;
    case LUA_BOOL:
        joes_set_var_bool_ref(tmp, lua_toboolean(l, 2));
        break;
    default:
        break;
    }
    return 0;
}

static int testf(lua_State *l)
{
    fputs("It has been called.\n", stderr);
    return 0;
}

/* Joestar interface functions to Lua. */
static const struct luaL_Reg libjoestar[] =
{
    { "jsync_l", l_sync_to_joestar },
    { "testf" , testf }
};

/* Internal Joestar variables. Can be set from Lua. */
static struct joe_var *joestar_var_names[] =
{
    &(struct joe_var){ "linum_mode", LUA_REAL,   true,  false, NULL, false },
    &(struct joe_var){ "undo_keep",  LUA_REAL,   true,  true,  NULL, false },
    &(struct joe_var){ "test_path",  LUA_STRING, false, false, NULL, false },
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
    (void)filepath;
    fprintf(stderr, "Lua failed!!!!: %s\n", lua_tostring(L, -1));
    /* TODO failstate */
}

/* Init LuaC */
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

    /*Register variables */
    for(size_t i = 0; i < sizeof(joestar_var_names) / sizeof(struct joe_var*); i++)
    {
        joes_add_var_by_ref(joestar_var_names[i]);
    }

    /* TODO for testing purposes we will simply load joesinit.lua. */
    run_lua_script("./init.lua");
    run_lua_script("./test.lua");

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

void jlua_var_sync(const char *name)
{

}


/* Syncs the joe_var value with the lua VM */
void jlua_var_sync_ref(struct joe_var *var)
{
    switch(var->type)
    {
    case LUA_STRING:
        lua_pushstring(L, var->str_value);
        break;
    case LUA_REAL:
        lua_pushnumber(L, var->num_value);
        break;
    case LUA_BOOL:
        lua_pushboolean(L, var->bool_value);
        break;
    default:
        break;
    }

    lua_setglobal(L, var->name);
}
