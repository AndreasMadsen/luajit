#ifndef ASYNC_H
#define ASYNC_H

#include <nan.h>
#include <uv.h>

template <class T>
void async_after(uv_work_t* req, int status);

#endif
