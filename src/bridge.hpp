#ifndef JOES_BRIDGE_HPP
#define JOES_BRIDGE_HPP

#include <string>
#include <string_view>
#include "joestar.h"

extern "C"
{
#include "types.h"
}


namespace jlua
{
    /* Get internal joe value by name */
    template<typename T>
    T getVal(std::string_view name);
    /* Set internal joe value */
    bool setVal(std::string_view name, int value);
    bool setVal(std::string_view name, bool value);
    bool setVal(std::string_view name, std::string_view value);
}

#endif /* JOES_BRIDGE_HPP */