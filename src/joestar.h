/*
 *  Joestar interface. Dual purpose C and C++ header file.
 *  Copyright
 *      (C) 2020 Ryan Jeffrey
 *
 *  This file is part of joestar.
 */

#ifndef JOESTAR_H
#define JOESTAR_H

/* C interface */
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void init_joestar(void);
void free_joestar(void);

#ifdef __cplusplus
}

/* C++ specific */
#include <stdexcept>
#include <string_view>

namespace jstar
{
    void log(const std::string_view msg);

    void err(const std::exception &e);
    void err(const std::string_view msg);
}

#endif // __cplusplus

#endif /* JOESTAR_H */