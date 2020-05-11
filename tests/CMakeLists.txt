enable_testing()

enable_language(CXX)
set(CMAKE_CXX_VERSION 11)


set(BINARY ${CMAKE_PROJECT_NAME}_test)

project(${BINARY})

add_executable(
    ${BINARY}
    main.cpp
    ublock.cpp
)

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(
    ${BINARY}
    # TODO the lib
    ${CMAKE_PROJECT_NAME}
    gtest
    gtest_main
    pthread # Maybe necessary?
)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
target_compile_options(${BINARY} PRIVATE
     $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
          -Wall -Wextra -Werror -Wconversion -Wsign-conversion>
     $<$<C_COMPILER_ID:MSVC>:
          /W4 /WX>)
endif()


