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
        bool isclosed;

    private:
        // Initialize and close
        static NAN_METHOD(New);
        static NAN_METHOD(Close);

        // Compile methods
        static NAN_METHOD(DoFile);
        static NAN_METHOD(DoString);

        // Stack operators
        static NAN_METHOD(Read);
        static NAN_METHOD(Pop);
        static NAN_METHOD(Push);
        static NAN_METHOD(GetTop);
        static NAN_METHOD(SetTop);
        static NAN_METHOD(PushValue);
        static NAN_METHOD(Remove);
        static NAN_METHOD(Insert);
        static NAN_METHOD(Replace);
};

#endif
