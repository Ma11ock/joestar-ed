#include "joestar.h"
extern "C"
{
#include "jlua.h"
}

void init_joestar(void)
{
    init_lua();
}

void end_joestar(void)
{
    free_lua();
}