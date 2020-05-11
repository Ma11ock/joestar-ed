enable_testing()

# gtest needs threads
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)


enable_language(CXX)
set(CMAKE_CXX_VERSION 11)
#include_directories("${PROJECT_SOURCE_DIR}/src/") It won't compile with this for some reason


set(BINARY ${CMAKE_PROJECT_NAME}_test)

project(${BINARY})

add_executable(
    ${BINARY}
    main.cpp
    ublock.cpp
)

target_link_libraries(
    ${BINARY}
    ${CMAKE_PROJECT_NAME}
    gtest
    gtest_main
    pthread
)

add_test(
    NAME    testfoo
    COMMAND ${BINARY}
)

# Auto run the test on make check
add_custom_target(check COMMAND ${BINARY}
                  DEPENDS ${BINARY})

if(CMAKE_BUILD_TYPE STREQUAL "debug")
target_compile_options(${BINARY} PRIVATE
     $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
          -Wall -Wextra -Werror -Wconversion -Wsign-conversion>
     $<$<C_COMPILER_ID:MSVC>:
          /W4 /WX>)
endif()

        