cmake_minimum_required(VERSION 3.10)

add_compile_definitions(JOERC="/usr/local/etc/joe")
add_compile_definitions(JOEDATA="/usr/share/joe")


project(joestar_lib)

enable_language(C)
set(CMAKE_C_VERSION 99)
cmake_policy(SET CMP0075 NEW)

add_subdirectory(src)
add_subdirectory(tests)

# TODO checking for operating system



# No need to set optimization levels as cmake will set the automatically depending
# on build type.
if (CMAKE_C_COMPILER_ID MATCHES "GNU")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()

if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -D_DEFAULT_SOURCE -D_GNU_SOURCE")
endif()


