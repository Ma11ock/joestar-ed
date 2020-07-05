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

#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
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

/* Wrapper for Lua type */
struct jlua_var
{
    jlua_type type;
};

typedef struct jlua_var jlua_var;

jlua_var  jlvar_init(jlua_type type);       /* JLVar Constructor         */
jlua_var *jlvar_malloc(jlua_type type);     /* JLVar Dynamic Constructor */
void jlvar_free_and_nil(jlua_var **var);    /* JLVar dynamic Destructor.
                                               Frees and sets to NULL    */

bool ensure_lua_type(jlua_var *var, jlua_type ltype); /* Ensure the lua type
                                                          of the wrapper struct
                                                          is the right type */


void init_lua();    /* Init Lua */
void free_lua();    /* End Lua  */

#endif /* JOES_LUA_H */