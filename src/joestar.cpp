#include "joestar.h"
#include "jlua.hpp"

#include <iostream>

extern "C"
{
#include "types.h"
}

/* Internal error buffer, never displayed directly */
static B *errBuf           = nullptr;
/* Buffer for displaying the contents of the error buffer, a copy of errBuf */
static B *curErrShowBuf    = nullptr;
/* True if curErrShowBuf is a valid pointer, false if not */
static bool showBuffExists = false;

/* opens BW with error log */
int ushowErrLog(W *w, int k)
{
    BW   *newbw;
    void *object;

    if(showBuffExists)
    {
        /* TODO place error log as active window */
    }
    else if(errBuf)
    {
        curErrShowBuf = bcpy(errBuf->bof, errBuf->eof);
        curErrShowBuf->name = zdup("* Error Log *");
        curErrShowBuf->internal = 1;

        newbw = (BW*) maint->curwin->object;
        object = newbw->object;
        w = newbw->parent;
        bwrm(newbw);
        w->object = (void*)(newbw = bwmk(w, curErrShowBuf, 0));
        wredraw(newbw->parent);
        newbw->object = object;
        showBuffExists = true;
        return 0;
    }
    return 1;
}

/* Inform joestar of the destruction of a buffer. Returns 1 if ok to destroy,
   returns 0 joestar needs the buffer */
int jstar_informDestroyBuffer(B *b)
{
    if(b->name != nullptr && !strcmp(b->name, "* Error Log *"))
    {
        showBuffExists = false;
    }
    return 1;
}

void createErrbuf()
{
    errBuf = bfind_scratch("* Jstar Error Log *");
    errBuf->internal = 1;
    //errBuf->scratch = 1;
    errBuf->current_dir = vsncpy(NULL, 0, NULL, 0);
}

void init_joestar(void)
{
    /* Initialize error buffer */
    createErrbuf();

    /* Initialize jlua */
    jlua::init();

    jstar::err("Kek");
}

void free_joestar(void)
{
    jlua::free();
    brm(errBuf);
    if(showBuffExists)
        brm(curErrShowBuf);
}

void jstar::log(const std::string_view msg)
{
}

void jstar::err(const std::exception &e)
{
    jstar::err(e.what());
}

void jstar::err(const std::string_view msg)
{
    ptrdiff_t msgLen = static_cast<ptrdiff_t>(msg.size());
    binsm(errBuf->eof, msg.data(), msgLen);
}