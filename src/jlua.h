#ifndef JOES_LUA_H
#define JOES_LUA_H

#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>

#include "joestar.h"

typedef enum
{
    LUA_NIL,        /* Nil                  */
    LUA_REAL,       /* Real number (float)  */
    LUA_STRING,     /* String               */
    LUA_TABLE,      /* Table                */
    LUA_BOOL,       /* Boolean              */
    LUA_FUNCTION    /* Function             */
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


jbool ensure_lua_type(jlua_var *var, jlua_type ltype); /* Ensure the lua type
                                                          of the wrapper struct
                                                          is the right type */

#endif /* JOES_LUA_H */