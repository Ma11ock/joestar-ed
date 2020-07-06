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
void add_var(struct joe_var var);
/* Create a hash from range 0-HASH_MAX for a string. Returns SIZE_MAX if key is NULL. */
size_t var_hash(const char *key);
/* Free a joe_var */
void joe_var_free(struct joe_var *var);
/* Destroys the var hash map */
void free_vars();
/* Get variable */
struct joe_var get_var_by_name(const char *name);

/* joe_var with default values */
extern const struct joe_var DEFAULT_JOE_VAR;

 #endif /* JOES_BRIDGE_H */