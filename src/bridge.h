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
/* Create a hash from range 0-HASH_MAX for a string */
ptrdiff_t var_hash(const char *key);
/* Create a new joe_var  */
struct joe_var *joe_var_init(const char *name);
/* Free a joe_var */
void joe_var_free(struct joe_var *var);
/* Destroys the var hash map */
void free_vars();



 #endif /* JOES_BRIDGE_H */