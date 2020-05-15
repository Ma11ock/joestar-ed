#ifndef JOES_LUA_H
#define JOES_LUA_H

#include <lua5.3/lua.h>
#include <lua5.3/lualib.h>

#include "joestar.h"

enum lua_type
{
    LUA_NIL,        /* Nil                  */
    LUA_REAL,       /* Real number (float)  */
    LUA_STRING,     /* String               */
    LUA_TABLE,      /* Table                */
    LUA_BOOL,       /* Boolean              */
    LUA_FUNCTION    /* Function             */
};

struct joes_var
{
    enum lua_type type;
};

struct joes_var jvar_init(enum lua_type type);
struct joes_var *jvar_malloc(enum lua_type type);
void jvar_free(struct joes_var *var);

jbool ensure_lua_type(struct joes_var *var, enum lua_type ltype);

#endif /* JOES_LUA_H */