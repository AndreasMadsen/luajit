
#include "baton.h"
#include "lua_state.h"

template <class T>
Baton<T>::Baton (v8::Arguments args, T const* userdata, int cbi) {
    L = node::ObjectWrap::Unwrap<LuaState>(args.This())->L;
    callback = new NanCallback(args[cbi].As<v8::Function>());
    data = userdata;
    failed = false;
}

template <class T>
Baton<T>::~Baton () {
    delete callback;
    delete[] data;
}

template <class T>
void Baton<T>::seterror (const char* msg) {
    failed = true;
    error = msg;
}

// This template will be used with <int> and <char> nothing else
template struct Baton<int>;
template struct Baton<char>;
