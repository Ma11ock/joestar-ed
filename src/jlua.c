#include "jlua.h"

#include "types.h"

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
struct jlua_var jlvar_init(jlua_type type)
{
    struct jlua_var result =
    {
        .type = type
    };
    return result;
}

/* JLVar Dynamic Constructor */
struct jlua_var *jlvar_malloc(jlua_type type)
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
