#ifndef JLUA_HPP
#define JLUA_HPP

extern "C"
{
#ifndef LUA_BUILT_WITH
#include <lua5.3/lua.h>
#include <lua5.3/lauxlib.h>
#include <lua5.3/lualib.h>

#else
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#endif /* LUA_BUILT_WITH */
}

#include <string>
#include <filesystem>


namespace jlua
{
    class var;

    enum class type
    {
        NIL        = LUA_TNIL,           /* Nil                       */
        BOOL       = LUA_TBOOLEAN,       /* Boolean                   */
        LIGHTUDATA = LUA_TLIGHTUSERDATA, /* Light User data           */
        REAL       = LUA_TNUMBER,        /* Real number (float)       */
        STRING     = LUA_TSTRING,        /* String                    */
        TABLE      = LUA_TTABLE,         /* Table                     */
        FUNCTION   = LUA_TFUNCTION,      /* Function                  */
        USERDATA   = LUA_TUSERDATA,      /* User data                 */
        THREAD     = LUA_TTHREAD,        /* Thread                    */
        INT,                             /* Integer                   */
    };

    void init();
    void free();

    void sync_vars(std::string &name);
    void sync_vars(var &v);

    bool do_file(std::filesystem::path path);
}

#endif /* JLUA_HPP */