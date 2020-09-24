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

typedef struct window W;
typedef struct buffer B;

/* Initialize internal joestar state */
void init_joestar(void);
/* Deallocate joestar state */
void free_joestar(void);
/* Display the error log */
int ushowErrLog(W *w, int k);
/* Inform joestar of the destruction of a buffer. Returns 1 if ok to destroy,
   returns 0 joestar needs the buffer */
int jstar_informDestroyBuffer(B *b);

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