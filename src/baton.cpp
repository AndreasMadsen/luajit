
#include "baton.h"
#include "lua_state.h"

Baton::Baton (v8::Arguments args) {
    L = node::ObjectWrap::Unwrap<LuaState>(args.This())->L;
    callback = new NanCallback(args[1].As<v8::Function>());
    data = NanCString(args[0], &datasize);
    failed = false;
}

Baton::~Baton () {
    delete callback;
    delete[] data;
}

void Baton::seterror (const char* msg) {
    failed = true;
    error = msg;
}
