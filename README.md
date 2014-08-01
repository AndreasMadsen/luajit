#luajit

> This module provides a basic binding to the Lua C interface. In most cases
> it uses LuaJIT 2.0 as the engine. _The exception to this is on Mac OS X with
> a 64 bit process, where LuaJIT is not compatible with node, instead Lua 5.2
> is used._

This is purly experimental. I'm writing this to learn Lua, Node C++ bindings
with a bit of libuv and Nan. Yes quite a mouthfull :p

### Example

**example.js**
```javascript
var path = require('path');
var luajit = require('luajit');

var state = new luajit.LuaState();
state.doFile(path.resolve(__dirname, 'example.lua'), function (error) {
    if (error) throw error;
});
state.close();
```

**example.lua**
```lua
print("Hallo from Lua")
```

### Install

This module is not yet published, but you can of cause clone it:
```shell
git clone --recursive https://github.com/AndreasMadsen/luajit.git
cd luajit
npm install
```

## API

You create a new state with:
```javascript
var state = new luajit.LuaState();
```
When you no longer need the state, close it with:
```javascript
state.close();
```

To manipulate the state use the following methods:

* [Compile Methods](#Compile-Methods) - Compile and run a string or file.
* [Global Operators](#Global-Operators) - Read and write the global table.
* [Stack Operators](#Stack-Operators) - Push, pop, manipulate and read the Lua stack.

### Compile Methods

Both methods compiles and run the code. When this is done the callback is called.
In case of errors (compile or runtime) the first argument in the callback will
be an error object, otherwise it is `null`.

* `LuaState.doFile(filepath, callback)` - Wraps `luaL_dofile` and reports error.
* `LuaState.doString(code, callback)` - Wraps `luaL_dostring` and reports error.

### Global Operators

* `LuaState.getGlobal(filepath, callback)` - Wraps `lua_getglobal`, push global value to stack.
* `LuaState.setGlobal(code, callback)` - Wraps `lua_setglobal`, put top stack element to global and pop stack.

### Stack Operators

* `value = LuaState.read(index)` - get a Lua by stack index and convert it to a v8 type.
* `LuaState.push(value)` - convert a v8 value to Lua and push it to the Lua stack.
* `length = LuaState.getTop()` - Wraps `lua_gettop`, returns the stack length.
* `LuaState.setTop(length)` - Wraps `Lua_settop`, set the stack length.
* `LuaState.pushValue(index)` - Wraps `lua_pushvalue`, push a copy of the given value by stack index.
* `LuaState.remove(index)` - Wraps `lua_remove`, remove element from stack.
* `LuaState.insert(index)` - Wraps `lua_insert`, move top element to new position in stack.
* `LuaState.replace(index)` - Wraps `lua_replace`, copy top value intro stack given by index.
