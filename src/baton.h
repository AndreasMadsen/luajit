#ifndef BATON_H
#define BATON_H

#include <v8.h>
#include <nan.h>

extern "C" {
    #include <lua.h>
}

template <class T>
struct Baton {
    lua_State* L;
    NanCallback* callback;

    T const* data;

    bool failed;
    const char* error;

    Baton (v8::Arguments args, T const* userdata, int cbi);
    ~Baton ();

    void seterror (const char* msg);
};

#endif
