/*
 *  The bridge between Lua and JOESTAR.
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of JOESTAR.
 */

#ifndef JOES_BRIDGE_H
#define JOES_BRIDGE_H

#include <stddef.h>
#include "jlua.h"

/* Prime number maximum hash value */
#define HASH_MAX 479UL

struct joe_var
{
    const char *name;

    jlua_type type;
    union
    {
        const char *str_value;
        double      num_value;
    };
};


/* Add a variable to the map */
void joes_add_var(struct joe_var var);
/* Create a hash from range 0-HASH_MAX for a string. Returns SIZE_MAX if key is NULL. */
size_t joes_var_hash(const char *key);
/* Free a joe_var */
void joe_var_free(struct joe_var *var);
/* Destroys the var hash map */
void joes_free_vars();
/* Get variable */
struct joe_var joes_get_var_by_name(const char *name);
/* Ensure the lua type of the wrapper struct is the right type */
bool ensure_lua_type(struct joe_var var, jlua_type ltype);

/* joe_var with default values */
extern const struct joe_var DEFAULT_JOE_VAR;

 #endif /* JOES_BRIDGE_H */