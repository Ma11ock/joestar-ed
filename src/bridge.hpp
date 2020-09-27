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
    template<typename T>
    T getVal(std::string_view name);

    template<typename T>
    bool setVal(std::string_view name, T value);
}

#endif /* JOES_BRIDGE_HPP */