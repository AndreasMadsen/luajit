
#include "lua_state.h"
#include "baton.h"
#include "async.h"
#include "lua_utils.h"
#include <stdio.h>

static v8::Persistent<v8::FunctionTemplate> luastate_constructor;

LuaState::LuaState () {
    L = luaL_newstate();
    luaL_openlibs(L);
    isclosed = false;
};

LuaState::~LuaState () {
    if (!isclosed) {
        lua_close(L);
        isclosed = true;
    }
};

void LuaState::Init (v8::Handle<v8::Object> target) {
    // Setup class
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(LuaState::New);
    NanAssignPersistent(luastate_constructor, tpl);
    tpl->SetClassName(NanNew<v8::String>("LuaState"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Setup prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "close", LuaState::Close);

    NODE_SET_PROTOTYPE_METHOD(tpl, "getGlobal", LuaState::GetGlobal);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setGlobal", LuaState::SetGlobal);

    NODE_SET_PROTOTYPE_METHOD(tpl, "doFile", LuaState::DoFile);
    NODE_SET_PROTOTYPE_METHOD(tpl, "doString", LuaState::DoString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "read", LuaState::Read);
    NODE_SET_PROTOTYPE_METHOD(tpl, "push", LuaState::Push);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pop", LuaState::Pop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTop", LuaState::SetTop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTop", LuaState::GetTop);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pushValue", LuaState::PushValue);
    NODE_SET_PROTOTYPE_METHOD(tpl, "remove", LuaState::Remove);
    NODE_SET_PROTOTYPE_METHOD(tpl, "insert", LuaState::Insert);
    NODE_SET_PROTOTYPE_METHOD(tpl, "replace", LuaState::Replace);

    // Export class constructor
    target->Set(NanNew<v8::String>("LuaState"), tpl->GetFunction());
}

//
// Initialize and close
//
NAN_METHOD(LuaState::New) {
    NanScope();

    LuaState* obj = new LuaState();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
}

NAN_METHOD(LuaState::Close) {
    LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());

    if (!obj->isclosed) {
        lua_close(obj->L);
        obj->isclosed = true;
    }

    NanReturnUndefined();
};

//
// Global operators
//
NAN_METHOD(LuaState::GetGlobal) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsString()) {
        v8::ThrowException(NanTypeError("LuaState.getGlobal first argument must be a string"));
    } else {
        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        size_t count;
        const char* string = NanCString(args[0], &count);
        lua_getglobal(obj->L, string);
        delete[] string;
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::SetGlobal) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsString()) {
        v8::ThrowException(NanTypeError("LuaState.setGlobal first argument must be a string"));
    } else {
        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        size_t count;
        const char* string = NanCString(args[0], &count);
        lua_setglobal(obj->L, string);
        delete[] string;
    }

    NanReturnUndefined();
}

//
// Compile methods
//
void do_file(uv_work_t* req) {
    Baton* baton = static_cast<Baton*>(req->data);

    if (luaL_dofile(baton->L, baton->data)) {
        baton->seterror(lua_tostring(baton->L, -1));
    }
}

NAN_METHOD(LuaState::DoFile) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsString()) {
        v8::ThrowException(NanTypeError("LuaState.doFile first argument must be a string"));
    } else if (args.Length() < 2 || !args[1]->IsFunction()) {
        v8::ThrowException(NanTypeError("LuaState.doFile second argument must be a function"));
    } else {
        Baton* baton = new Baton(args);

        uv_work_t *req = new uv_work_t;
        req->data = baton;
        uv_queue_work(uv_default_loop(), req, do_file, async_after);
    }

    NanReturnUndefined();
}

void do_string(uv_work_t* req) {
    Baton* baton = static_cast<Baton*>(req->data);

    if (luaL_dostring(baton->L, baton->data)) {
        baton->seterror(lua_tostring(baton->L, -1));
    }
}

NAN_METHOD(LuaState::DoString) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsString()) {
        v8::ThrowException(NanTypeError("LuaState.doString first argument must be a string"));
    } else if (args.Length() < 2 || !args[1]->IsFunction()) {
        v8::ThrowException(NanTypeError("LuaState.doString second argument must be a function"));
    } else {
        Baton* baton = new Baton(args);

        uv_work_t *req = new uv_work_t;
        req->data = baton;
        uv_queue_work(uv_default_loop(), req, do_string, async_after);
    }

    NanReturnUndefined();
}

//
// Stack operators
//
NAN_METHOD(LuaState::Read) {
    NanScope();
    v8::Handle<v8::Value> value = NanUndefined();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.read first argument must be a number"));
    } else {
        bool supportedType;
        int index = (int)args[0]->ToNumber()->Value();

        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        value = lua_to_v8(obj->L, index, &supportedType);
        if (!supportedType) {
            v8::ThrowException(NanTypeError("LuaState.read couldn't convert stack value to v8"));
        }
    }

    NanReturnValue(value);
}

NAN_METHOD(LuaState::Push) {
    NanScope();

    if (args.Length() < 1) {
        v8::ThrowException(NanTypeError("LuaState.push requires 1 argument"));
    } else {
        bool supportedType;

        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        push_v8_to_lua(obj->L, args[0], &supportedType);
        if (!supportedType) {
            v8::ThrowException(NanTypeError("LuaState.push couldn't convert argument to lua"));
        }
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::Pop) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.pop first argument must be a number"));
    } else {
        int num = (int)args[0]->ToNumber()->Value();
        if (num < 0) {
            v8::ThrowException(NanRangeError("LuaState.pop first argument must be positive"));
        } else {
            LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
            lua_pop(obj->L, num);
        }
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::GetTop) {
    NanScope();

    LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
    int n = lua_gettop(obj->L);

    NanReturnValue(NanNew<v8::Number>(n));
}

NAN_METHOD(LuaState::SetTop) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.setTop first argument must be a number"));
    } else {
        int index = (int)args[0]->ToNumber()->Value();

        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        lua_settop(obj->L, index);
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::PushValue) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.pushValue first argument must be a number"));
    } else {
        int index = (int)args[0]->ToNumber()->Value();

        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        lua_pushvalue(obj->L, index);
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::Remove) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.remove first argument must be a number"));
    } else {
        int index = (int)args[0]->ToNumber()->Value();
        if (index < 0) {
            v8::ThrowException(NanRangeError("LuaState.remove first argument must be positive"));
        } else {
            LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
            lua_remove(obj->L, index);
        }
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::Insert) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.insert first argument must be a number"));
    } else {
        int index = (int)args[0]->ToNumber()->Value();
        if (index < 0) {
            v8::ThrowException(NanRangeError("LuaState.insert first argument must be positive"));
        } else {
            LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
            lua_insert(obj->L, index);
        }
    }

    NanReturnUndefined();
}

NAN_METHOD(LuaState::Replace) {
    NanScope();

    if (args.Length() < 1 || !args[0]->IsNumber()) {
        v8::ThrowException(NanTypeError("LuaState.replace first argument must be a number"));
    } else {
        int index = (int)args[0]->ToNumber()->Value();

        LuaState* obj = node::ObjectWrap::Unwrap<LuaState>(args.This());
        lua_replace(obj->L, index);
    }

    NanReturnUndefined();
}
