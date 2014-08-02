
var test = require('tape');
var luajit = require('../luajit.js');
var temp = require('temp');
var fs = require('fs');

test('compile by string', function (t) {
    t.test('- missing code string', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.doString(null, function () {});
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.doString first argument must be a string');
        state.close();
        t.end();
    });

    t.test('- missing callback', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.doString('', null);
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.doString second argument must be a function');
        state.close();
        t.end();
    });

    t.test('- compile error', function (t) {
        var state = new luajit.LuaState();
        state.doString('syntax error', function (error) {
            t.equal(error.name, 'Error');
            t.equal(error.message, "[string \"syntax error\"]:1: syntax error near 'error'");
            t.equal(state.getTop(), 0);

            state.close();
            t.end();
        });
    });

    t.test('- successful execution', function (t) {
        var state = new luajit.LuaState();
        state.doString('x = "string from lua"', function (error) {
            t.strictEqual(error, null);
            state.getGlobal('x');
            t.strictEqual(state.read(-1), 'string from lua');
            state.close();
            t.end();
        });
    });

    t.end();
});

test('compile by file', function (t) {
    t.test('- missing code string', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.doFile(null, function () {});
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.doFile first argument must be a string');
        state.close();
        t.end();
    });

    t.test('- missing callback', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.doFile('', null);
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.doFile second argument must be a function');
        state.close();
        t.end();
    });

    t.test('- compile error', function (t) {
        var scriptpath = temp.path({suffix: '.lua'});
        fs.writeFile(scriptpath, 'syntax error', function (err) {
            t.ifError(err);

            var state = new luajit.LuaState();
            state.doFile(scriptpath, function (error) {
                t.equal(error.name, 'Error');
                t.equal(error.message.slice(-33), ".lua:1: syntax error near 'error'");
                t.equal(state.getTop(), 0);

                state.close();

                fs.unlink(scriptpath, function (err) {
                    t.ifError(err);
                    t.end();
                });
            });
        });
    });

    t.test('- successful execution', function (t) {
        var scriptpath = temp.path({suffix: '.lua'});
        fs.writeFile(scriptpath, 'x = "string from lua"', function (err) {
            t.ifError(err);

            var state = new luajit.LuaState();
            state.doFile(scriptpath, function (error) {
                t.strictEqual(error, null);
                state.getGlobal('x');
                t.strictEqual(state.read(-1), 'string from lua');

                state.close();

                fs.unlink(scriptpath, function (err) {
                    t.ifError(err);
                    t.end();
                });
            });
        });
    });

    t.end();
});

test('call Lua method', function (t) {
    t.test('- missing parameter number', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.call(null, 1, function () {});
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.call first argument must be a number');
        state.close();
        t.end();
    });

    t.test('- missing return number', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.call(2, null, function () {});
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.call second argument must be a number');
        state.close();
        t.end();
    });

    t.test('- missing callback', function (t) {
        var state = new luajit.LuaState();
        var error = null;
        try {
            state.call(2, 1, null);
        } catch (e) { error = e; }
        t.equal(error.name, 'TypeError');
        t.equal(error.message, 'LuaState.call thrid argument must be a function');
        state.close();
        t.end();
    });

    t.test('- bad execution', function (t) {
        var state = new luajit.LuaState();
        state.doString('function test () error(\'Failed\') end', function (err) {
            t.ifError(err);

            state.getGlobal('test');
            state.push(5);
            state.push(3);
            state.call(2, 1, function (err) {
                t.equal(err.name, 'Error');
                t.equal(err.message,'[string "function test () error(\'Failed\') end"]:1: Failed');
                t.equal(state.getTop(), 0);

                state.close();
                t.end();
            });
        });
    });

    t.test('- successful execution', function (t) {
        var state = new luajit.LuaState();
        state.doString('function addition (x,y) return x+y end', function (err) {
            t.ifError(err);

            state.getGlobal('addition');
            state.push(5);
            state.push(3);
            state.call(2, 1, function (err) {
                t.ifError(err);
                t.strictEqual(state.read(-1), 8);

                state.close();
                t.end();
            });
        });
    });

    t.end();
});
