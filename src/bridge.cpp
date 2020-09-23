#include "bridge.hpp"

///////////////////////////////////////////////////////////////////////////////
//                                   Var                                     //
///////////////////////////////////////////////////////////////////////////////

jlua::Var::Var(const char *name, const char *description,
               bool isBufLocal)
                   : _bufLocal(isBufLocal),_name(name),_descr(description)
{
}

std::string jlua::Var::getName() const
{
    return _name;
}

std::string jlua::Var::describe() const
{
    return _descr;
}

std::string jlua::Var::getOpt() const
{
    char *tmp = strdup(_name.c_str());
    if(!tmp)
    {
        // TODO
    }
    std::string result = get_status((BW*)maint->curwin->object, tmp);
    if(result == "???" || result == "") /*TODO error*/
    {
    }

    free(tmp);
    return result;
}
