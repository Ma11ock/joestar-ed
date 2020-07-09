/*
 *  Basic Lua interface utils.
 *
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of joestar.
 */

#ifndef JOES_LUA_H
#define JOES_LUA_H

#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>
#include <stdbool.h>

#include "joestar.h"

typedef enum
{
    LUA_NIL        = LUA_TNIL,           /* Nil                  */
    LUA_BOOL       = LUA_TBOOLEAN,       /* Boolean              */
    LUA_LIGHTUDATA = LUA_TLIGHTUSERDATA, /* Light User data      */
    LUA_REAL       = LUA_TNUMBER,        /* Real number (float)  */
    LUA_STRING     = LUA_TSTRING,        /* String               */
    LUA_TABLE      = LUA_TTABLE,         /* Table                */
    LUA_FUNCTION   = LUA_TFUNCTION,      /* Function             */
    LUA_USERDATA   = LUA_TUSERDATA,      /* User data            */
    LUA_THREAD     = LUA_TTHREAD         /* Thread               */
} jlua_type;

struct joe_var;

void init_lua();    /* Init Lua */
void free_lua();    /* End Lua  */
void jlua_set_string(const char *var_name, const char *str);
void jlua_var_sync(const char *name);
void jlua_var_sync_ref(struct joe_var *var);

#endif /* JOES_LUA_H */