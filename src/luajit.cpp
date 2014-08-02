
#include <v8.h>
#include "lua_state.h"

void Init (v8::Handle<v8::Object> target) {
    LuaState::Init(target);
}

NODE_MODULE(bindings, Init)
