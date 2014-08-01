
var test = require('tape');
var luajit = require('../luajit.js');

test('stack push', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.push();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.push requires 1 argument');
        state.close();
        t.end();
    });

    t.test('- unsupported v8 type', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.push(undefined);
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, "LuaState.push couldn't convert argument to lua");
        state.close();
        t.end();
    });

    t.end();
});

test('stack read', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.read();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.read first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- out of range', function (t) {
        var state = new luajit.LuaState();
        t.strictEqual(state.read(1), undefined);
        state.close();
        t.end();
    });

    t.end();
});

test('type convertion', function (t) {

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

    t.test('- null', function (t) {
        var state = new luajit.LuaState();
        state.push(null);
        t.strictEqual(state.read(-1), null);
        state.close();
        t.end();
    });

    t.end();
});

test('stack pop', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.pop();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.pop first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- negative number', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.pop(-1);
        } catch (e) { error = e; }
        t.equal(error.name, 'RangeError');
        t.equal(error.message, 'LuaState.pop first argument must be positive');
        state.close();
        t.end();
    });

    t.test('- pop one element', function (t) {
        var state = new luajit.LuaState();
        state.push(1);
        state.push(2);
        t.strictEqual(state.read(-1), 2);
        state.pop(1);
        t.strictEqual(state.read(-1), 1);
        state.close();
        t.end();
    });

    t.end();
});

test('stack gettop', function (t) {
    var state = new luajit.LuaState();
    state.push(true);
    t.strictEqual(state.getTop(), 1);
    state.push(true);
    t.strictEqual(state.getTop(), 2);
    state.close();
    t.end();
});

test('stack settop', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.setTop();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.setTop first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- simple', function (t) {
        var state = new luajit.LuaState();
        state.push(true);
        state.setTop(2);
        t.strictEqual(state.read(-1), null);
        t.strictEqual(state.read(-2), true);
        state.setTop(1);
        t.strictEqual(state.read(-1), true);
        state.close();
        t.end();
    });

    t.end();
});

test('stack pushValue', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.pushValue();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.pushValue first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- simple', function (t) {
        var state = new luajit.LuaState();
        state.push(1);
        state.pushValue(1);
        t.strictEqual(state.read(-1), 1);
        t.strictEqual(state.read(-2), 1);
        state.close();
        t.end();
    });

    t.end();
});

test('stack remove', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.remove();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.remove first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- negative number', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.remove(-1);
        } catch (e) { error = e; }
        t.equal(error.name, 'RangeError');
        t.equal(error.message, 'LuaState.remove first argument must be positive');
        state.close();
        t.end();
    });

    t.test('- simple', function (t) {
        var state = new luajit.LuaState();
        state.push(1);
        state.push(2);
        state.remove(2);
        t.strictEqual(state.read(-1), 1);
        t.strictEqual(state.getTop(), 1);
        state.close();
        t.end();
    });

    t.end();
});

test('stack insert', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.insert();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.insert first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- negative number', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.insert(-1);
        } catch (e) { error = e; }
        t.equal(error.name, 'RangeError');
        t.equal(error.message, 'LuaState.insert first argument must be positive');
        state.close();
        t.end();
    });

    t.test('- simple', function (t) {
        var state = new luajit.LuaState();
        state.push(1);
        state.push(2);
        state.insert(1);
        t.strictEqual(state.read(-1), 1);
        t.strictEqual(state.read(-2), 2);
        state.close();
        t.end();
    });

    t.end();
});

test('stack replace', function (t) {
    t.test('- no argument', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.replace();
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.replace first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- simple', function (t) {
        var state = new luajit.LuaState();
        state.push(1);
        state.push(2);
        state.push(3);
        state.replace(1);
        t.strictEqual(state.read(-1), 2);
        t.strictEqual(state.read(-2), 3);
        state.close();
        t.end();
    });

    t.end();
});
