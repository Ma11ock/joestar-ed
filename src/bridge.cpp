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
    constexpr std::size_t MAX_VAR_NAME_SIZE = 64;


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

        std::string_view getOpt() const
        {
            char *tmp = new char[_name.size() + 1];
            std::size_t i = 0;
            for(char c : _name)
            {
                tmp[i] = c;
                i++;
            }

            std::string result;
            try
            {
                result = get_status((BW*)maint->curwin->object, tmp);
            }
            catch(std::exception &e)
            {
                /*TODO error*/
                std::cerr << "Opt " << _name << " has get'd NULL\n";
                delete []tmp;
                return "";
            }
            delete []tmp;

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

        jlua::type getType() const
        {
            return _luaType;
        }

        void setVal();
    };

    using varMap = std::unordered_map<std::string_view, ::Var>;
    /* Joestar's internal variables */
    static ::varMap vars
    {
        { "undo_keep", { "undo_keep", "test", jlua::type::INT, false } },
        { "linum_mode", { "linum_mode",  "test2", jlua::type::INT, true } },
        { "eval_path", { "eval_path", "test3", jlua::type::STRING, false } },
    };

    template<typename T, unsigned long N = std::numeric_limits<T>::max( )>
    class MaxLen
    {
    public:
        enum
        {
            StringLen = MaxLen< T, N / 10 >::StringLen + 1
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

    using intString  = char[::MaxLen<int>::StringLen + 1];
    using offtString = char[::MaxLen<off_t>::StringLen + 1];
    using boolString = char[::MaxLen<bool>::StringLen + 1];
    using varString  = char[::MAX_VAR_NAME_SIZE + 1];
}



///////////////////////////////////////////////////////////////////////////////
//                               Public Getters                              //
///////////////////////////////////////////////////////////////////////////////


template<>
bool jlua::setVal<int>(std::string_view name, int newInt)
{
    ::intString intStr;
    ::varString varStr;
    if(std::snprintf(intStr, sizeof(intStr), "%d", newInt) < 0)
    {
        std::cerr << "Error: Could not encode " << newInt << '\n';
        return false;
    }

    std::strncpy(varStr, name.data(), name.size());

    glopt(varStr, intStr, NULL, 1);
    return true;
}

template<>
int jlua::getVal<int>(std::string_view name)
{
    constexpr int WRONG_DEFAULT = -1;
    int result = 0;
    ::varMap::const_iterator tmp = vars.find(name );
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

    auto err = std::from_chars(name.data(), name.data() + name.size(), result);
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

    int result;
    auto err = std::from_chars(name.data(), name.data() + name.size(), result);
    if(err.ec == std::errc::invalid_argument)
    {
        /*TODO*/
        std::cerr << "Could not convert " << name << " to bool\n";
        return WRONG_DEFAULT;
    }

    return static_cast<bool>(result);
}

template<>
std::string jlua::getVal<std::string>(std::string_view name)
{
    const char *WRONG_DEFAULT = "";
    ::varMap::const_iterator tmp = vars.find(name);
    std::string result;
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

    try
    {
        result = tmp->second.getOpt();
    }
    catch(std::invalid_argument &e)
    {
        std::cerr << "The string is wrong\n";
        return WRONG_DEFAULT;
    }

    return result;
}

