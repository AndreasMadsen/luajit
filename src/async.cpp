
#include "async.h"
#include "baton.h"

template <class T>
void async_after(uv_work_t* req, int status) {
    NanScope();

    Baton<T>* baton = static_cast<Baton<T>*>(req->data);

    v8::Handle<v8::Value> argv[] = { NanNull() };
    if (baton->failed) {
        argv[0] = NanError(baton->error);
    }
    baton->callback->Call(1, argv);

    delete baton;
    delete req;
}

template void async_after<char>(uv_work_t*, int);
template void async_after<int>(uv_work_t*, int);
