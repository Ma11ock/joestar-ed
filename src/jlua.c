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
    case LUA_FUNCTION:
        result = lua_isfunction(l, index);
        break;
    default: /*Nil*/
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
        if(joes_set_var_real_ref(tmp, lua_tonumber(l, 2)))
        {
            jlua_var_sync_ref(tmp);
        }
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
bool run_lua_from_string(const char *name)
{
    if(luaL_dostring(L, name) != LUA_OK)
    {
        fprintf(stderr, "Lua failed: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); /* pop error message */
        return false;
    }

    return true;
}

void run_lua_script(const char *filepath)
{
    if(luaL_dofile(L, filepath) != LUA_OK)
    {
        goto lua_fail;
    }
	return;

lua_fail:
    fprintf(stderr, "Lua failed: %s\n", lua_tostring(L, -1));
    lua_pop(L, 1); /* pop error message */
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
    run_lua_from_string("joestar = require 'joestar'\n");
}

/* End Lua  */
void free_lua()
{
    lua_close(L);
    L = NULL;
    joes_free_vars(); /* destroy joestar builtins */
}

/* Check the Lua state to make sure there are no errors. */
static bool check_lua(int r)
{
    bool result = true;
    if(r != LUA_OK)
    {
        fprintf(stderr, "Error in lua: %s\n", lua_tostring(L, -1)); // TODO handle error
        lua_pop(L, 1); /* pop error message */
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

/*Sync the value of the joe_var to its lua counterpart*/
void jlua_var_sync(const char *name)
{
    jlua_var_sync_ref(joes_get_var_by_name(name));
}

/* Syncs the joe_var value with the lua VM */
void jlua_var_sync_ref(struct joe_var *var)
{

    switch(var->type)
    {
    case LUA_STRING:
        jlua_set_var_str(var->name, var->str_value);
        break;
    case LUA_REAL:
        jlua_set_var_real(var->name, var->num_value);
        break;
    case LUA_BOOL:
        jlua_set_var_bool(var->name, var->bool_value);
        break;
    default:
//        lua_pushnil(L);
        break;
    }
}


int jlua_eval_block(W *w, int k)
{
    (void)k;
    BW *bw;
    WIND_BW(bw, w);

    char *tmp = blkget(bw);
    bool result = run_lua_from_string(tmp);
    joe_free(tmp);

    return (int)result;
}

int jlua_eval_buffer(W *w, int k)
{
    (void)k;
    BW *bw;
    WIND_BW(bw, w);

    char *tmp = blkget_pts(bw, bw->b->bof, bw->b->eof);
    bool result = run_lua_from_string(tmp);
    joe_free(tmp);

    return (int)result;
}

int jlua_eval_line(W *w, int k)
{
    (void)k;
    BW *bw;
    WIND_BW(bw, w);
    const char *iden = "jlua_eval_line";

    P *tmp_bol = pdup(bw->cursor, iden);
    P *tmp_eol = pdup(bw->cursor, iden);

    p_goto_bol(tmp_bol);
    p_goto_eol(tmp_eol);

    char *tmp = blkget_pts(bw, tmp_bol, tmp_eol);
    bool result = run_lua_from_string(tmp);

    joe_free(tmp);
    prm(tmp_bol);
    prm(tmp_eol);

    return (int)result;
}

/* get number variable from lua vm. error will be set to true on error, and it will return -INF */
double jlua_get_global_real(const char *name, bool *error)
{
    double result = -INFINITY;
    *error = false;

    jlua_get_global("get_variable", LUA_FUNCTION);

    if(!lua_isfunction(L, -1))
    {
        /*TODO error*/
    }

    lua_pushstring(L, name);
    if(check_lua(lua_pcall(L, 1, 1, 0)))
    {
        if(!lua_isnumber(L, -1))
        {
            *error = true;
        }
        else
        {
            result = lua_tonumber(L, -1);
        }

    }
    else
    {
        *error = true;
    }

    lua_pop(L, 1); /* pop the result and joe table */

    return result;
}

/* Set the lua variable name to the value of real */
void jlua_set_var_real(const char *name, double real)
{
    jlua_get_global("variable_sync", LUA_FUNCTION);

    if(!lua_isfunction(L, -1))
    {
        /*TODO error*/
    }

    lua_pushstring(L, name);
    lua_pushnumber(L, real);
    if(!check_lua(lua_pcall(L, 2, 0, 0)))
    {
        /*TODO*/
    }

}

/* Set the lua variable name to the value of b */
void jlua_set_var_bool(const char *name, bool b)
{
    jlua_get_global("variable_sync", LUA_FUNCTION);

    if(!lua_isfunction(L, -1))
    {
        /*TODO error*/
    }

    lua_pushstring(L, name);
    lua_pushboolean(L, b);
    if(!check_lua(lua_pcall(L, 2, 0, 0)))
    {
        /*TODO*/
    }
}

/* Set the lua variable name to the value of str */
void jlua_set_var_str(const char *name, const char *str)
{
    jlua_get_global("variable_sync", LUA_FUNCTION);

    if(!lua_isfunction(L, -1))
    {
        /*TODO error*/
    }

    lua_pushstring(L, name);
    lua_pushstring(L, str);
    if(!check_lua(lua_pcall(L, 2, 0, 0)))
    {
        /*TODO*/
    }
}

/* gets a global and puts it on the lua stack, ensuring it is of type 'type' */
void jlua_get_global(const char *name, jlua_type type)
{
    fprintf(stderr, "Getting the global: %s\n", name);
    lua_getglobal(L, name);
    fprintf(stderr, "With the value: %s\n", lua_tostring(L, -1));

    if(!types_match(type, L, -1))
    {
        exit(1);
        /* TODO big error */
    }
}
