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
    const char *name;         /* Name of the variable                                         */
    jlua_type   type;         /* Type interpretation of data                                  */
    bool        int_data;     /* True if var represents internal joe data (glopt)             */
    bool        global;       /* True if global variable, false if local (to buffer)          */
    union                     /* Data                                                         */
    {
        const char *str_value;
        double      num_value;
        bool        bool_value;
    };

    bool free;                /* true if ownership (the responsibility to free()) is had by
                                 bridge.c                                                     */
    char *descr;              /* description string */
};

extern struct joe_var usermail;
extern struct joe_var username;
extern struct joe_var linum;
extern struct joe_var undokeep;

/* Add a variable to the map */
void joes_add_var(const char *name, jlua_type type, bool int_data, bool global, const char *descr);
/* Add a variable to the map */
void joes_add_var_struct(struct joe_var var);
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
/* var getters */
/* gets string value from variable 'name'. Returns NULL if no variable, or if not set */
const char *joes_get_vstring(const char *name);
/* gets number value from variable 'name'. Returns NAN if no variable, or if not set  */
double joes_get_vreal(const char *name);
/* gets bool value from variable 'name'. Returns false if no variable, or if not set  */
bool joes_get_vbool(const char *name);
/* setters */
void joes_set_var_string(const char *name, const char *str);
void joes_set_var_bool(const char *name, bool boolean);
bool joes_set_var_real(const char *name, double real);

void joes_set_var_string_ref(struct joe_var *var, const char *str);
void joes_set_var_bool_ref(struct joe_var *var, bool boolean);
/* joe internal integer variables can have bounds or other limits on what they can be,
   so not all number values are valid. joes_set_var_real_ref returns true if the value is not valid,
   and the lua VM needs to be synced. */
bool joes_set_var_real_ref(struct joe_var *var, double real);
void joes_var_unset(const char *name);
void joes_var_unset_ref(struct joe_var *var);
/* joe_var with default values */
extern const struct joe_var DEFAULT_JOE_VAR;

 #endif /* JOES_BRIDGE_H */