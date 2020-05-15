#include "lua.h"

#include "types.h"

jbool ensure_lua_type(struct joes_var *var, enum lua_type ltype)
{
    jbool result = JFALSE;
    if(var->type == ltype)
    {
        result = JTRUE;
    }

    return result;
}

struct joes_var jvar_init(enum lua_type type)
{
    struct joes_var result =
    {
        .type = type
    };
    return result;
}

struct joes_var *jvar_malloc(enum lua_type type)
{
    struct joes_var *newVar = joe_malloc(sizeof(struct joes_var));
    *newVar = jvar_init(type);

    return newVar;
}

void jvar_free(struct joes_var *var)
{
    joe_free(var);
}
