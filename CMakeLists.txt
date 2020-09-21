cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "release")
endif()
message("Building for ${CMAKE_BUILD_TYPE}")

if(UNIX AND NOT APPLE)
# Linux
    add_compile_definitions(JOERC="/usr/local/etc/joestarrc")
    add_compile_definitions(JOEDATA="/usr/share/joestar")
elseif(WIN32)
    add_compile_definitions(JOERC="C:\\Program Files\\joestarrc")
    add_compile_definitions(JOEDATA="C:\\Program Files\\joestar")
endif()


project(joestar)

include(CMakeDetermineSystem)
include(ExternalProject)

if(JOESTAR_BUILD_LUA)
    ExternalProject_Add(lua
        URL "https://www.lua.org/ftp/lua-5.3.5.tar.gz"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND make generic
        BUILD_ALWAYS true
        BUILD_IN_SOURCE true
        INSTALL_COMMAND ""
    )
    add_library(liblua STATIC IMPORTED)
else()
    find_package(Lua 5.3 REQUIRED)
endif()

enable_language(C)
enable_language(CXX)
enable_testing()
set(CMAKE_C_VERSION 99)
set(CMAKE_CXX_VERSION 17)
cmake_policy(SET CMP0075 NEW)

add_subdirectory(src)
add_subdirectory(tests)

add_compile_definitions(_DEFAULT_SOURCE=1)
add_compile_definitions(_GNU_SOURCE=1)
