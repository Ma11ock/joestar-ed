cmake_minimum_required(VERSION 3.10)

set(default_build_type "debug")
message("Building for ${CMAKE_BUILD_TYPE}")

if(UNIX AND NOT APPLE)
# Linux
    add_compile_definitions(JOERC="/usr/local/etc/joestarrc")
    add_compile_definitions(JOEDATA="/usr/share/joestar")
elseif(WIN32)
    add_compile_definitions(JOERC="C:\\Program Files\\joestarrc")
    add_compile_definitions(JOEDATA="C:\\Program Files\\joestar")
endif()


project(joestar_lib)

enable_language(C)
set(CMAKE_C_VERSION 99)
cmake_policy(SET CMP0075 NEW)

add_subdirectory(src)
add_subdirectory(tests)

# No need to set optimization levels as cmake will set the automatically depending
# on build type.
if (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()
