#include "joestar.h"
extern "C"
{
#include "jlua.h"
}

void init_joestar(void)
{
    init_lua();
}

void free_joestar(void)
{
    free_lua();
}
