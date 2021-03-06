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
state.doFile(path.resolve(__dirname, 'example.lua'), function (err) {
    if (err) throw err;

    state.getGlobal('addition');
    state.push(5);
    state.push(3);
    state.call(2, 1, function (err) {
        // Any error will be caught and then converted intro a Error object.
        // Furemore the error message have been poped from the Lua stack, so
        // you propagate without worrying.
        if (err) throw err;

        console.log(state.read(-1)); // print 8
        state.pop(1); // lets be nice and cleanup the returned value

        state.close();
    });
});
```

**example.lua**
```lua
function addition (x,y)
    return x+y
end
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

* [Execution Methods](#execution-methods) - Compile and run a string or file.
* [Global Operators](#global-operators) - Read and write the global table.
* [Stack Operators](#stack-operators) - Push, pop, manipulate and read the Lua stack.

### Execution Methods

Both `LuaState.doFile` and `LuaState.doString` compiles and run the code. When
this is done you can call a function using `LuaState.call`.

All these functions will report any error (compile or runtime) as the first
argument in the callback. If there was an error the error message will also have
been poped from the Lua stack, this is to allow you to easily propergate the
error object without worrying about leaking the error message.

If everything went fine then the first argument will be `null`.

* `LuaState.doFile(filepath, callback)` - Wraps `luaL_dofile` and reports error.
* `LuaState.doString(code, callback)` - Wraps `luaL_dostring` and reports error.
* `LuaState.call(params, returns, callback)` - Wraps `lua_pcall` and reports error.

### Global Operators

* `LuaState.getGlobal(name)` - Wraps `lua_getglobal`, push global value to stack.
* `LuaState.setGlobal(name)` - Wraps `lua_setglobal`, put top stack element to global and pop stack.

### Stack Operators

* `value = LuaState.read(index)` - get a Lua by stack index and convert it to a v8 type.
* `LuaState.push(value)` - convert a v8 value to Lua and push it to the Lua stack.
* `LuaState.pop(length)` - Wraps `lua_pop`, pop `length` elements from the top of the stack.
* `length = LuaState.getTop()` - Wraps `lua_gettop`, returns the stack length.
* `LuaState.setTop(length)` - Wraps `Lua_settop`, set the stack length.
* `LuaState.pushValue(index)` - Wraps `lua_pushvalue`, push a copy of the given value by stack index.
* `LuaState.remove(index)` - Wraps `lua_remove`, remove element from stack.
* `LuaState.insert(index)` - Wraps `lua_insert`, move top element to new position in stack.
* `LuaState.replace(index)` - Wraps `lua_replace`, copy top value intro stack given by index.
