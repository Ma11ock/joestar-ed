#include "bridge.h"

#include "types.h"

struct joe_var_node
{
    struct joe_var_node *next;
    struct joe_var var;
};

static struct joe_var_node *var_hash_map[HASH_MAX] = {NULL};

struct joe_var_node *make_node(struct joe_var var)
{
    struct joe_var_node *new_node =
        (struct joe_var_node*)joe_malloc(sizeof(struct joe_var_node));

    new_node->next = NULL;
    new_node->var = var;
    return new_node;
}

/* Add the variable var to the list at local */
void add_var_to_list(struct joe_var var, ptrdiff_t local)
{
    struct joe_var_node *head = var_hash_map[local];
    struct joe_var_node *tmp  = NULL;

    /* No variable at location */
    if(head == NULL)
    {
        head = make_node(var);
    }
    /* If a list, add var to a new node at the end of the list */
    else
    {
        while(head->next != NULL)
        {
            head = head->next;
        }

        head->next = make_node(var);
    }
}


/* Create a hash from range 0-HASH_MAX for a string */
ptrdiff_t var_hash(const char *key)
{
    ptrdiff_t sum = 0;
    for(char c = *key++; c != '\0'; c = *key++)
    {
        sum += (ptrdiff_t)c * HASH_MAX;
    }

    return sum % (ptrdiff_t)HASH_MAX;
}

/* Create a new joe_var */
struct joe_var *joe_var_init(const char *name)
{
    struct joe_var *new_var = (struct joe_var*)joe_malloc(sizeof(struct joe_var));
    new_var->name = name;
    new_var->type = LUA_NIL;
    return new_var;
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
        }
    }
}