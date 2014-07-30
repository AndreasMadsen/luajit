#ifndef LUA_STATE_H
#define LUA_STATE_H

#include <v8.h>
#include <node.h>
#include <nan.h>
#include <uv.h>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

class LuaState : public node::ObjectWrap {
    public:
        static void Init(v8::Handle<v8::Object> target);
        LuaState();
        ~LuaState();

        lua_State* L;

    private:
        static NAN_METHOD(New);
        static NAN_METHOD(doFile);
};

#endif
