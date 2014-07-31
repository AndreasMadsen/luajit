
#include "lua_utils.h"

v8::Handle<v8::Value> lua_to_v8(lua_State* L, int i, bool* supportedType) {
    *supportedType = true;

    switch(lua_type(L, i)){
        case LUA_TBOOLEAN:
            return NanNew<v8::Boolean>((bool)lua_toboolean(L, i));
            break;
        case LUA_TNUMBER:
            return NanNew<v8::Number>(lua_tonumber(L, i));
            break;
        case LUA_TSTRING:
            return NanNew<v8::String>(lua_tostring(L, i));
            break;
        case LUA_TNIL:
            return NanNull();
            break;
        case LUA_TNONE:
            return NanUndefined();
            break;
        case LUA_TTABLE:
        case LUA_TFUNCTION:
        case LUA_TUSERDATA:
        case LUA_TTHREAD:
        case LUA_TLIGHTUSERDATA:
        default:
            *supportedType = false;
            return NanUndefined();
            break;
    }
}

void push_v8_to_lua(lua_State* L, v8::Handle<v8::Value> value, bool* supportedType) {
    *supportedType = true;

    if (value->IsString()) {
        size_t count;
        char* string = NanCString(value, &count);
        lua_pushstring(L, string);
        delete[] string;
    }
    else if (value->IsNumber()) {
        int i_value = value->ToNumber()->Value();
        lua_pushinteger(L, i_value);
    }
    else if (value->IsBoolean()) {
        int b_value = (int)value->ToBoolean()->Value();
        lua_pushboolean(L, b_value);
    }
    else if (value->IsObject()) {
        lua_pushnil(L);
    }
    else if (value->IsNull()) {
        lua_pushnil(L);
    }
    else {
        *supportedType = false;
    }
}
