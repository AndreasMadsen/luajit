#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <nan.h>
#include <v8.h>

extern "C" {
    #include <lua.h>
}

v8::Handle<v8::Value> lua_to_v8(lua_State* L, int i, bool* supportedType);
void push_v8_to_lua(lua_State* L, v8::Handle<v8::Value> value, bool* supportedType);

#endif
