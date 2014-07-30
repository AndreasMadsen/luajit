
#include "lua_state.h"

struct Baton {
    lua_State* L;
    NanCallback* callback;

    size_t datasize;
    char* data;

    bool failed;
    const char* error;

    Baton (v8::Arguments args) {
        L = node::ObjectWrap::Unwrap<LuaState>(args.This())->L;
        callback = new NanCallback(args[1].As<v8::Function>());
        data = NanCString(args[0], &datasize);
        failed = false;
    }
    ~Baton () {
        delete callback;
        delete[] data;
    }

    void seterror (const char* msg) {
        failed = true;
        error = msg;
    }
};

static v8::Persistent<v8::FunctionTemplate> luastate_constructor;

LuaState::LuaState () {
    L = luaL_newstate();
    luaL_openlibs(L);
};

LuaState::~LuaState () {
    lua_close(L);
};

void LuaState::Init (v8::Handle<v8::Object> target) {
    // Setup class
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(LuaState::New);
    NanAssignPersistent(luastate_constructor, tpl);
    tpl->SetClassName(NanNew<v8::String>("LuaState"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Setup prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "doFile", LuaState::doFile);

    // Export class constructor
    target->Set(NanNew<v8::String>("LuaState"), tpl->GetFunction());
}

NAN_METHOD(LuaState::New) {
    NanScope();

    LuaState* obj = new LuaState();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
}

void do_file(uv_work_t *req){
    Baton* baton = static_cast<Baton*>(req->data);

    if (luaL_dofile(baton->L, baton->data)) {
        baton->seterror(lua_tostring(baton->L, -1));
    }
}

void async_after(uv_work_t *req, int status){
    NanScope();

    Baton* baton = static_cast<Baton*>(req->data);

    v8::Handle<v8::Value> argv[] = { NanNull(), NanNull() };
    if (baton->failed) {
        argv[0] = NanError(baton->error);
    }
    baton->callback->Call(2, argv);

    delete baton;
}

NAN_METHOD(LuaState::doFile) {
    NanScope();

    Baton* baton = new Baton(args);

    uv_work_t *req = new uv_work_t;
    req->data = baton;
    uv_queue_work(uv_default_loop(), req, do_file, async_after);

    NanReturnUndefined();
}
