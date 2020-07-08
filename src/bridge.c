#include "bridge.h"

#include "types.h"

#include <string.h>
#include <stdint.h>

struct joe_var usermail = { "UserMail", LUA_STRING, NULL, true, NULL, false };
struct joe_var username = { "UserName", LUA_STRING, NULL, true, NULL, false };


struct joe_var_node
{
    struct joe_var_node *next;
    struct joe_var      *var;
};

static struct joe_var_node *var_hash_map[HASH_MAX];

const struct joe_var DEFAULT_JOE_VAR = { NULL, LUA_NIL, NULL, false, NULL };

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
                joe_free(head->var);
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
void joes_add_var(const char *name, jlua_type type, void *data, bool global)
{
    struct joe_var *newVar = (struct joe_var*)joe_malloc(sizeof(struct joe_var));

    *newVar = (struct joe_var)
              { .name = name,
                .type = type,
                .data = data,
                .global = global,
                .str_value = NULL,
                .free = true };
    add_var_to_list(newVar, joes_var_hash(name));
}



/* Joestar variables */
/* (really wish i could hash these at compile time...) */
void joes_init_bridge()
{
    joes_add_var_by_ref(&usermail);
    joes_add_var_by_ref(&username);
}

/* Add variable by reference */
void joes_add_var_by_ref(struct joe_var *var)
{
    add_var_to_list(var, joes_var_hash(var->name));
}

/* Add a variable to the map */
void joes_add_var_struct(struct joe_var var)
{
    joes_add_var(var.name, var.type, var.data, var.global);
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
        tmp->data = NULL;
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
    if(name == NULL || str == NULL)
        return;

    struct joe_var *tmp = joes_get_var_by_name(name);
    /*TODO log these errors*/
    if(tmp == NULL || tmp->type != LUA_STRING)
        return;

    /* Set data if necessary */
    if(tmp->data != NULL)
    {
        tmp->data = str;
    }

    tmp->str_value = NULL;
}

/* setter for reference  */
void joes_set_val_ref(struct joe_var *var, const void *data)
{

}
