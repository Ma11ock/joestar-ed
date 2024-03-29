#include "bridge.h"

#include "types.h"

#include <string.h>
#include <stdint.h>
#include <math.h>

struct joe_var_node
{
    struct joe_var_node *next;
    struct joe_var      *var;
};

static struct joe_var_node *var_hash_map[HASH_MAX];

const struct joe_var DEFAULT_JOE_VAR = { NULL, LUA_NIL, NULL, false, NULL, false, NULL };

struct joe_var_node *make_node(struct joe_var *var)
{
     struct joe_var_node *new_node =
        (struct joe_var_node*)joe_malloc(sizeof(struct joe_var_node));

    new_node->next = NULL;
    new_node->var = var;
    return new_node;
}

/* Add the variable var to the list at local */
void add_var_to_list(struct joe_var *var, size_t local)
{
    struct joe_var_node **head = &var_hash_map[local];

    /* No variable at location */
    if(*head == NULL)
    {
        *head = make_node(var);
    }
    /* If a list, add var to a new node at the end of the list */
    else
    {
        while((*head)->next != NULL)
        {
            head = &((*head)->next);
        }

        (*head)->next = make_node(var);
    }
}


/* Create a hash from range 0-HASH_MAX for a string. Returns SIZE_MAX if key is NULL. */
size_t joes_var_hash(const char *key)
{
    if(key == NULL)
        return SIZE_MAX;

    size_t sum = 0;
    for(char c = *key++; c != '\0'; c = *key++)
    {
        sum += (size_t)c * HASH_MAX;
    }

    return sum % (size_t)HASH_MAX;
}

/* Destroys the var hash map */
void joes_free_vars()
{
    struct joe_var_node *head = NULL;
    struct joe_var_node *tmp  = NULL;
    for(size_t i = 0; i < (size_t)HASH_MAX; i++)
    {
        head = var_hash_map[i];
        while(head != NULL)
        {
            /* Traverse and free the list */
            tmp = head->next;
            if(head->var->free)
            {
                if(head->var->descr)
                    free(head->var->descr);
                joe_free(head->var);
            }
            joe_free(head);
            head = tmp;
            var_hash_map[i] = NULL;
        }
    }

}

/* Get joe_var by its name. */
struct joe_var *joes_get_var_by_name(const char *name)
{
    if(name == NULL)
        return NULL;

    struct joe_var_node *head   = var_hash_map[joes_var_hash(name)];
    struct joe_var      *result = NULL;

    while(head != NULL)
    {

        if(strcmp(head->var->name, name) == 0)
        {
            result = head->var;
            head = NULL;
        }
        else
        {
            head = head->next;
        }
    }

    return result;
}

/* Ensure the lua type of the wrapper struct is the right type */
bool ensure_lua_type(struct joe_var *var, jlua_type ltype)
{
    return (var->type == ltype);
}

/* Add a variable to the map */
void joes_add_var(const char *name, jlua_type type, bool int_data, bool global, const char *descr)
{
    struct joe_var *newVar = (struct joe_var*)joe_malloc(sizeof(struct joe_var));

    *newVar = (struct joe_var)
              { .name = name,
                .type = type,
                .int_data = int_data,
                .global = global,
                .str_value = NULL,
                .free = true,
                .descr = strdup((descr) ? descr : ""),
                .origin = (int_data) ? "Joe's Own Editor Internal Data" : "" };
    add_var_to_list(newVar, joes_var_hash(name));
}



/* Add variable by reference */
void joes_add_var_by_ref(struct joe_var *var)
{
    add_var_to_list(var, joes_var_hash(var->name));
}

/* Add a variable to the map */
void joes_add_var_struct(struct joe_var var)
{
    joes_add_var(var.name, var.type, var.int_data, var.global, var.descr);
}

/* gets string value from variable 'name' */
const char *joes_get_vstring(const char *name)
{
    struct joe_var *tmp = joes_get_var_by_name(name);

    return (tmp == NULL) ? NULL : tmp->str_value;
}

/* gets number value from variable 'name' */
double joes_get_vreal(const char *name)
{
    struct joe_var *tmp = joes_get_var_by_name(name);

    return (tmp == NULL) ? NAN : tmp->num_value;
}

/* gets bool value from variable 'name' */
bool joes_get_vbool(const char *name)
{
    struct joe_var *tmp = joes_get_var_by_name(name);

    return (tmp == NULL) ? false : tmp->bool_value;
}

/* setter */
void joes_set_val(const char *name, const void *data)
{
    if(name == NULL)
        return;

    struct joe_var *tmp = joes_get_var_by_name(name);

    if(data == NULL)
    {
        tmp->str_value = NULL;
        tmp->int_data = false;
        return;
    }

    switch(tmp->type)
    {
    case LUA_STRING:
        break;
    default:
        break;
    }
}

void joes_set_var_string(const char *name, const char *str)
{
    joes_set_var_string_ref(joes_get_var_by_name(name), str);
}

bool joes_set_var_real(const char *name, double real)
{
    return joes_set_var_real_ref(joes_get_var_by_name(name), real);
}

void joes_set_var_bool(const char *name, bool boolean)
{
    joes_set_var_bool_ref(joes_get_var_by_name(name), boolean);
}

void joes_set_var_string_ref(struct joe_var *var, const char *str)
{
    /*TODO log these errors*/
    if(var == NULL || var->type != LUA_STRING || str == NULL)
        return;

    /* Set data if necessary */
    if(var->int_data)
    {
        glopt(var->name, str, NULL, 1);
    }

    var->str_value = str;
}

void joes_set_var_bool_ref(struct joe_var *var, bool boolean)
{
    /*TODO log these errors*/
    if(var == NULL || var->type != LUA_BOOL)
        return;

    /* Set data if necessary */
    if(var->int_data)
    {
        char str_bool[2] = { '\0' };
        str_bool[0] = (!boolean) ? '0' : '1';
        glopt(var->name, str_bool, NULL, 1);
    }

    var->bool_value = boolean;

}

bool joes_set_var_real_ref(struct joe_var *var, double real)
{
    /*TODO log these errors*/
    if(var == NULL || var->type != LUA_REAL)
        return false;

    bool should_sync = false;

    /* Set data if necessary */
    if(var->int_data)
    {
        double tmp = var->num_value;
        /* joe glopts only take integers, so no need to keep value as double */
        char int_string[33];
        snprintf(int_string, sizeof(int_string), "%d", (int)real);
        glopt(var->name, int_string, NULL, true);
        /* Check if actually set (number could be OOB) */
        int option_value = get_option_value(var->name);
        real = floor((option_value == (int)real) ? real : tmp);
        should_sync = (real != tmp) ? true : false;
    }

    var->num_value = real;
    return should_sync;
}

void joes_var_unset(const char *name)
{
    joes_var_unset_ref(joes_get_var_by_name(name));
}

void joes_var_unset_ref(struct joe_var *var)
{
    var->str_value = NULL;

    if(var->int_data)
        glopt(var->name, NULL, NULL, 1);
}

/* Prints all info about a joe_var to a dynamic buffer. Returns NULL on error. */
char *joes_var_to_str(const char *name)
{
    return joes_var_ref_to_str(joes_get_var_by_name(name));
}

char *joes_var_value_to_str(const char *name)
{
    return joes_var_value_to_str_ref(joes_get_var_by_name(name));
}

char *joes_var_value_to_str_ref(struct joe_var *var)
{
    char *result = NULL;
    switch(var->type)
    {
    case LUA_STRING:
        result = strdup((var->str_value) ? var->str_value : _("(null)"));
        break;
    case LUA_REAL:
#       define MAX_DBL_STR_SIZ 1080
        result = (char*)malloc(MAX_DBL_STR_SIZ);
        snprintf(result, MAX_DBL_STR_SIZ, "%f", var->num_value);
        break;
    case LUA_BOOL:
        result = strdup((var->bool_value) ? _("true") : _("false"));
        break;
    default: /*Nil*/
        result = strdup(_("Nil"));
        break;
    }

    return result;
}

char *joes_var_ref_to_str(struct joe_var *var)
{
    if(var == NULL)
        return NULL;

    const char *type_to_str = jlua_type_to_str(var->type);
    char *value_str = joes_var_value_to_str_ref(var);
    size_t size = strlen(var->name) + strlen(var->descr) +
        strlen(var->origin) + strlen(type_to_str) + strlen(value_str) +
        sizeof(_("\n\nDescription:\nIs a variable defined in:\nIt's value Is:It's type is:\n()\n")) + 6;

    char *result = (char*)joe_malloc(size);
    result[0] = '\0'; /* strcat will write garbage to first few bytes otherwise */
    if(result == NULL)
        return NULL;

    strcat(result, var->name);
    strcat(result, _(" is a variable defined in: "));
    strcat(result, var->origin);
    strcat(result, _("\nIts value is: "));
    strcat(result, value_str);
    strcat(result, _(" ("));
    strcat(result, type_to_str);
    strcat(result, _(")\n"));
    strcat(result, _("\n\nDescription:\n"));
    strcat(result, var->descr);

    result[size - 1] = '\0'; /* just in case ... */

    free(value_str);
    return result;
}
