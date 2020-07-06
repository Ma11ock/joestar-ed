#include "bridge.h"

#include "types.h"

#include <string.h>
#include <stdint.h>

struct joe_var_node
{
    struct joe_var_node *next;
    struct joe_var var;
};

static struct joe_var_node *var_hash_map[HASH_MAX];

const struct joe_var DEFAULT_JOE_VAR = { NULL, LUA_NIL };

struct joe_var_node *make_node(struct joe_var var)
{
     struct joe_var_node *new_node =
        (struct joe_var_node*)joe_malloc(sizeof(struct joe_var_node));

    new_node->next = NULL;
    new_node->var = var;
    return new_node;
}

/* Add the variable var to the list at local */
void add_var_to_list(struct joe_var var, size_t local)
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
size_t var_hash(const char *key)
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

/* Add a variable to the map */
void add_var(struct joe_var var)
{
    add_var_to_list(var, var_hash(var.name));
}

/* Destroys the var hash map */
void free_vars()
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
            joe_free(head);
            head = tmp;
            var_hash_map[i] = NULL;
        }
    }

}

/* Get joe_var by its name. */
struct joe_var get_var_by_name(const char *name)
{
    if(name == NULL)
        return DEFAULT_JOE_VAR;

    struct joe_var_node *head   = var_hash_map[var_hash(name)];
    struct joe_var       result = DEFAULT_JOE_VAR;

    while(head != NULL)
    {

        if(strcmp(head->var.name, name) == 0)
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
