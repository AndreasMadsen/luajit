
var test = require('tape');
var luajit = require('../luajit.js');

test('push then read', function (t) {

    t.test('- boolean', function (t) {
        var state = new luajit.LuaState();
        state.push(true);
        t.strictEqual(state.read(-1), true);
        state.close();
        t.end();
    });

    t.test('- number', function (t) {
        var state = new luajit.LuaState();
        state.push(10);
        t.strictEqual(state.read(-1), 10);
        state.close();
        t.end();
    });

    t.test('- string', function (t) {
        var state = new luajit.LuaState();
        state.push("hallo");
        t.strictEqual(state.read(-1), "hallo");
        state.close();
        t.end();
    });

    t.end();
});
