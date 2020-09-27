#include "bridge.hpp"
#include "jlua.hpp"
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <charconv>
#include <cstring>
#include <cstdio>
#include <limits>

namespace
{
    /* Maximum size for a variable length in JOE (without NUL terminator) */
    constexpr std::size_t MAX_VAR_NAME_SIZE = 64;

    /* For determing the buffer size of string representing and integer */
    template<typename T, unsigned long N = std::numeric_limits<T>::max()>
    class MaxLen
    {
    public:
        enum
        {
            StringLen = MaxLen<T, N / 10>::StringLen + 1
        };
    };

    template<typename T>
    class   MaxLen<T, 0>
    {
    public:
        enum
        {
            StringLen = 1
        };
    };

    /* Local type aliases. These help to avoid allocations. */
    using intString  = char[::MaxLen<int>::StringLen + 1];
    using offtString = char[::MaxLen<off_t>::StringLen + 1];
    using boolString = char[::MaxLen<bool>::StringLen + 1];
    using varString  = char[::MAX_VAR_NAME_SIZE + 2]; /* could have '-' in front*/

    /* Represents internal joe state. */
    class Var
    {
    private:
        const std::string_view _name;        /* the name of the internal joe varible,
                                                and the lua variable                 */
        const std::string_view _descr;       /* description string of variable       */
        const jlua::type       _luaType;     /* Lua type                             */
        const bool             _bufLocal;    /* true if the variable is buffer local */

    public:
        Var(const char *name, const char *description, jlua::type luaType,
            bool isBufLocal = false)
            : _name(name),_descr(description),_luaType(luaType),
                _bufLocal(isBufLocal)
        {}

        ~Var() = default;

        /* Gets joe's variable internal state as a string_view */
        std::string_view getOpt() const
        {
            ::varString tmp;
            std::strncpy(tmp, _name.data(), _name.size());
            tmp[_name.size()] = '\0';

            /* TODO NULL will segfault if not global option */
            std::string_view result = get_status(NULL, tmp);
            if(result == "" || result == "???")
            {
                /*TODO error*/
                std::cerr << "Opt " << _name << " has get'd NULL\n";
                return "";
            }

            if(result == "???")
                return "";

            return result;
        }


        /* Get variable's description (from lua func describe()) */
        std::string_view describe() const
        {
            return _descr;
        }

        /* true if the variable is not initialized, false if the variable has
           a value */
        bool isNil() const
        {
            return false;
        }

        /* Get Type */
        jlua::type getType() const
        {
            return _luaType;
        }

    };

    /* Map of variables */
    using varMap = std::unordered_map<std::string_view, ::Var>;
    /* Joestar's internal variables */
    static ::varMap vars
    {
        { "undo_keep", { "undo_keep", "test", jlua::type::INT, false } },
        { "linum_mode", { "linum_mode",  "test2", jlua::type::INT, true } },
        { "eval_path", { "eval_path", "test3", jlua::type::STRING, false } },
    };

}

///////////////////////////////////////////////////////////////////////////////
//                               Public Getters                              //
///////////////////////////////////////////////////////////////////////////////


template<>
int jlua::getVal<int>(std::string_view name)
{
    constexpr int WRONG_DEFAULT = -1;
    int result = 0;
    ::varMap::const_iterator tmp = vars.find(name);
    if(tmp == vars.end())
    {
        std::cerr << "Could not find " << name << '\n';
        return WRONG_DEFAULT;
    }

    const ::Var var = tmp->second;
    if(var.getType() != jlua::type::INT)
    {
        std::cerr << name << " is not an integer.\n";
        return WRONG_DEFAULT;
    }

    std::string_view opt = var.getOpt();
    if(opt == "" || opt == "???")
    {
        std::cerr << "Unable to internally find var " << name << '\n';
        return WRONG_DEFAULT;
    }

    auto err = std::from_chars(opt.data(), opt.data() + opt.size(), result);
    if(err.ec == std::errc::invalid_argument)
    {
        std::cerr << "Could not convert " << opt << " to an integer\n";
        return WRONG_DEFAULT;
    }
    return result;
}

template<>
bool jlua::getVal<bool>(std::string_view name)
{
    constexpr bool WRONG_DEFAULT = false;
    ::varMap::const_iterator tmp = vars.find(name);
    if(tmp == vars.end())
    {
        std::cerr << name << " is unknown.\n";
        return WRONG_DEFAULT;
    }

    if(tmp->second.getType() != jlua::type::BOOL)
    {
        std::cerr << name << " is not a bool\n";
        return WRONG_DEFAULT;
    }

    std::string_view opt = tmp->second.getOpt();
    int result;
    auto err = std::from_chars(opt.data(), opt.data() + opt.size(), result);
    if(err.ec == std::errc::invalid_argument)
    {
        /*TODO*/
        std::cerr << "Could not convert " << name << " to bool\n";
        return WRONG_DEFAULT;
    }

    return static_cast<bool>(result);
}

template<>
std::string_view jlua::getVal<std::string_view>(std::string_view name)
{
    const char *WRONG_DEFAULT = "";
    ::varMap::const_iterator tmp = vars.find(name);
    std::string_view result;
    if(tmp == vars.end())
    {
        std::cerr << "Unable to find var " << name << '\n';
        return WRONG_DEFAULT;
    }

    if(tmp->second.getType() != jlua::type::STRING)
    {
        std::cerr << "Var is not a string\n";
        return WRONG_DEFAULT;
    }

    result = tmp->second.getOpt();
    if(result == "")
    {
        std::cerr << "Could not get " << name << "'s string\n";
        result = WRONG_DEFAULT;
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////
//                               Public Setters                              //
///////////////////////////////////////////////////////////////////////////////


bool jlua::setVal(std::string_view name, int newInt)
{
    ::intString intStr;
    ::varString varStr;
    if(std::snprintf(intStr, sizeof(intStr), "%d", newInt) < 0)
    {
        std::cerr << "Error: Could not encode " << newInt << '\n';
        return false;
    }

    std::strncpy(varStr, name.data(), name.size());
    varStr[name.size()] = '\0';

    glopt(varStr, intStr, NULL, 1);

    return (newInt == getVal<int>(name));
}

bool jlua::setVal(std::string_view name, std::string_view val)
{
    ::varString varStr;
    std::strncpy(varStr, name.data(), name.size());
    varStr[name.size()] = '\0';

    char *valueStr = new char[val.size() + 1];
    std::strncpy(valueStr, val.data(), val.size());

    glopt(varStr, valueStr, NULL, 1);
    delete []valueStr;
    return (val == getVal<std::string_view>(name));
}

bool jlua::setVal(std::string_view name, bool bVal)
{
    ::varString varStr;
    char *beginStr = bVal ? varStr : varStr + 1;
    if(beginStr != varStr)
    {
        varStr[0] = '-';
        varStr[name.size() + 1] = '\0';
    }
    else
        varStr[name.size()] = '\0';

    std::strncpy(beginStr, name.data(), name.size());
    glopt(varStr, NULL, NULL, 1);
    return (bVal == getVal<bool>(name));
}

