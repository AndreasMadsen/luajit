
#include "async.h"
#include "baton.h"

void async_after(uv_work_t* req, int status) {
    NanScope();

    Baton* baton = static_cast<Baton*>(req->data);

    v8::Handle<v8::Value> argv[] = { NanNull() };
    if (baton->failed) {
        argv[0] = NanError(baton->error);
    }
    baton->callback->Call(1, argv);

    delete baton;
    delete req;
}
