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
    const char *name;   /* Name of the variable                                         */
    jlua_type   type;   /* Type interpretation of data                                  */
    void       *data;   /* Pointer to global data. Same as set in glopt (see options.c).
                           NULL if maps to no glopt                                     */
    bool        global; /* True if global variable, false if local (to buffer)          */
    union       value   /* Data                                                         */
    {
        const char *str_value;
        double      num_value;
        bool        toggle;
    };
};

/* Add a variable to the map */
void joes_add_var(const char *name, jlua_type type, void *data, bool global, void *value);
/* Add variable by reference */
void joes_add_var_by_ref(struct joe_var *var);
/* Create a hash from range 0-HASH_MAX for a string. Returns SIZE_MAX if key is NULL. */
size_t joes_var_hash(const char *key);
/* Destroys the var hash map */
void joes_free_vars();
/* Get variable */
struct joe_var *joes_get_var_by_name(const char *name);
/* Ensure the lua type of the wrapper struct is the right type */
bool ensure_lua_type(struct joe_var *var, jlua_type ltype);
/* initialize the internal joestar variables */
void joes_init_bridge();

/* joe_var with default values */
extern const struct joe_var DEFAULT_JOE_VAR;

 #endif /* JOES_BRIDGE_H */