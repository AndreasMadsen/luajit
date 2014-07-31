#ifndef BATON_H
#define BATON_H

#include <v8.h>
#include <node.h>
#include <nan.h>

extern "C" {
    #include <lua.h>
}

struct Baton {
    lua_State* L;
    NanCallback* callback;

    size_t datasize;
    char* data;

    bool failed;
    const char* error;

    Baton (v8::Arguments args);
    ~Baton ();

    void seterror (const char* msg);
};

#endif
