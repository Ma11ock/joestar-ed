#include "joestar.h"
#include "jlua.hpp"

#include <iostream>

void init_joestar(void)
{
    jlua::init();
}

void free_joestar(void)
{
    jlua::free();
}

void jstar::log(const std::string_view msg)
{
}

void jstar::err(const std::exception &e)
{
    std::cerr << e.what() << '\n';
}

void jstar::err(const std::string_view msg)
{
}