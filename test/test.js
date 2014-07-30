
var path = require('path');
var luajit = require('../luajit.js');

console.log('Before');
var state = new luajit.LuaState();
state.doFile(path.resolve(__dirname, 'test.lua'), function () {
    console.log(arguments);
});
console.log('After');
