#include "joestar.h"
#include "jlua.hpp"

#include <iostream>
#include <tuple>
#include <vector>

extern "C"
{
#include "types.h"
}

/* Joestar's internal buffers */
static std::vector<jstar::InternalBuffer> internalBuffers =
{
    jstar::InternalBuffer(),  /* Message buffer */
    jstar::InternalBuffer(),  /* Error buffer   */
};

jstar::InternalBuffer &msgBuf = internalBuffers[0];
jstar::InternalBuffer &errBuf = internalBuffers[1];

jstar::InternalBuffer::InternalBuffer()
    : _realBuf(nullptr),_copyBuf(nullptr),_hasCopyBuf(false)
{
}

/* Kept separate from Constructor so it can initialize after joe's
   internal state */
void jstar::InternalBuffer::init(std::string_view name)
{
    _realBuf = bfind_scratch(name.data());
    _realBuf->internal = 1;
    //_realBuf->scratch = 1;
    _realBuf->current_dir = vsncpy(NULL, 0, NULL, 0);
}

/* delete the buffers, is kept separate from destructor so it can
   deallocate memory before joe deallocates it own internal state  */
void jstar::InternalBuffer::free()
{
    brm(_realBuf);
    _realBuf = nullptr;
    if(_hasCopyBuf)
        brm(_copyBuf);
    _copyBuf = nullptr;
    _hasCopyBuf = false;
}

/* Write to the buffers */
jstar::InternalBuffer &jstar::InternalBuffer::operator<<
    (const std::string_view msg)
{
    ptrdiff_t msgLen = static_cast<ptrdiff_t>(msg.size());
    binsm(_realBuf->eof, msg.data(), msgLen);
    if(_hasCopyBuf)
        binsm(_copyBuf->eof, msg.data(), msgLen);
    return *this;
}

jstar::InternalBuffer &jstar::InternalBuffer::operator<<
    (const std::exception &e)
{
    return ((*this) << e.what());
}

/* Show the buffer in a window */
bool jstar::InternalBuffer::show(W *w)
{
    BW   *newbw;
    void *object;

    if(_hasCopyBuf)
    {
        /* TODO place error log as active window */
    }
    else if(_realBuf)
    {
        _copyBuf = bcpy(_realBuf->bof, _realBuf->eof);
        _copyBuf->name = zdup(_realBuf->name);
        _copyBuf->internal = 1;

        newbw = (BW*) maint->curwin->object;
        object = newbw->object;
        w = newbw->parent;
        bwrm(newbw);
        w->object = (void*)(newbw = bwmk(w, _copyBuf, 0));
        wredraw(newbw->parent);
        newbw->object = object;
        _hasCopyBuf = true;
        return false;
    }
    return true;

}

/* Inform joestar that a buffer is going to be destroyed */
void jstar::InternalBuffer::informOfBufferDestroy(std::string_view bufName)
{
    if(_hasCopyBuf && bufName == _copyBuf->name)
        _hasCopyBuf = false;
}

/* opens BW with error log */
int ushowErrLog(W *w, int k)
{
    return static_cast<int>(errBuf.show(w));
}

/* Inform joestar of the destruction of a buffer. Returns 1 if ok to destroy,
   returns 0 joestar needs the buffer */
int jstar_informDestroyBuffer(B *b)
{
    if(b->name)
        for(auto &ib : internalBuffers)
            ib.informOfBufferDestroy(b->name);
    return 1;
}

void init_joestar(void)
{
    /* Initialize jlua */
    jlua::init();

    errBuf.init("* Error Log *");
    msgBuf.init("* Messages *");
}

void free_joestar(void)
{
    jlua::free();
    errBuf.free();
    msgBuf.free();
}

