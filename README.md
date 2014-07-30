#luajit

> This module provides a basic binding to the Lua C interface. In most cases
> it uses LuaJIT 2.0 as the engine. _The exception to this is on Mac OS X with
> a 64 bit process, where LuaJIT is not compatible with node, instead Lua 5.2
> is used._

This is purly experimental. I'm writing this to learn Lua, Node C++ bindings
with a bit of libuv and Nan. Yes quite a mouthfull :p

**example.js**
```javascript
var path = require('path');
var luajit = require('luajit');

var state = new luajit.LuaState();
state.doFile(path.resolve(__dirname, 'example.lua'), function (error) {
    if (error) throw error;
});
```

**example.lua**
```lua
print("Hallo from Lua")
```
