
var test = require('tape');
var luajit = require('../luajit.js');
var fs = require('fs');

test('global operators', function (t) {
    t.test('- setGlobal no arguments', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.setGlobal();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.setGlobal first argument must be a string');
        state.close();
        t.end();
    });

    t.test('- getGlobal no arguments', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.getGlobal();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.getGlobal first argument must be a string');
        state.close();
        t.end();
    });

    t.test('set and get global', function (t) {
        var state = new luajit.LuaState();

        state.push(1);
        state.push(2);
        state.setGlobal('x');
        t.strictEqual(state.read(-1), 1);
        state.getGlobal('x');
        t.strictEqual(state.read(-1), 2);

        state.close();
        t.end();
    });

    t.end();
});
