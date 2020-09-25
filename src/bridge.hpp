#ifndef JOES_BRIDGE_HPP
#define JOES_BRIDGE_HPP

#include <string>
#include <string_view>
#include <stdexcept>
#include "joestar.h"

extern "C"
{
#include "types.h"
}

namespace jlua
{

     /* Dummy class that represents a joestar variable */
    class Var
    {
    private:
        bool              _bufLocal;   /* true if the variable is buffer local */
        const std::string _name;       /* the name of the variable (in lua)    */
        const std::string _descr;      /* description string of variable       */

        std::string getOpt() const;

    public:
        Var(const char *name, const char *description,
            bool isBufLocal = false);
        ~Var() = default;

        /* Get variable's name in Lua */
        std::string getName() const;
        /* Get variable's description (from lua func describe()) */
        std::string describe() const;

        template<typename T>
        T getVal() const;

    };

    template<>
    int Var::getVal<int>() const
    {
        int result = 0;
        std::string opt = getOpt();
        try
        {
            result = std::stoi(opt);
        }
        catch(const std::invalid_argument &e)
        {
        }
        return result;
    }

    template<>
    bool Var::getVal<bool>() const
    {
        return false;
    }

    template<>
    std::string Var::getVal<std::string>() const
    {
        return "";
    }


}

#endif /* JOES_BRIDGE_HPP */