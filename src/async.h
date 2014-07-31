#ifndef ASYNC_H
#define ASYNC_H

#include <uv.h>
#include <nan.h>

void async_after(uv_work_t* req, int status);

#endif
