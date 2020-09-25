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
#include <sstream>

namespace jstar
{
    enum class Errno
    {
        WARNING,
        FATAL
    };

    /* possibilities: one B for window, objects for actual logging, search objects
                      One ebuffer per regular joestar buffer
                      Could literally be an org mode file (B*), and you edit it like you would edit joe */
    /* Wrapper for joe buffers used by joestar for internal data and logging */
    class InternalBuffer
    {
    private:
        B          *_realBuf;    /* Buffer for writing         */
        B          *_copyBuf;    /* Copy buffer for displaying */
        bool        _hasCopyBuf; /* true if copyBuf is a valid
                                    pointer, false if not      */

    public:
        InternalBuffer();
        ~InternalBuffer() = default;

        /* delete the buffers, is kept separate from destructor so it can
           deallocate memory before joe deallocates it own internal state  */
        void free();
        /* Write to the buffers */
        InternalBuffer &operator<<(const std::string_view msg);
        InternalBuffer &operator<<(const std::exception &e);
        /* Show the buffer in a window */
        bool show(W *w);
        /* Inform joestar that a buffer is going to be destroyed */
        void informOfBufferDestroy(std::string_view bufName);
        /* Kept separate from Constructor so it can initialize after joe's
           internal state */
        void init(std::string_view name);
    };
    /* Message buffer (analogous to stdout) */
    extern InternalBuffer& msgBuf;
    /* Error buffer (analogous to stderr) */
    extern InternalBuffer& errBuf;
}

#endif // __cplusplus

#endif /* JOESTAR_H */