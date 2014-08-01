
var test = require('tape');
var luajit = require('../luajit.js');
var temp = require('temp');
var fs = require('fs');

test('compile by string', function (t) {
    t.test('- compile error', function (t) {
        var state = new luajit.LuaState();
        state.doString('syntax error', function (error) {
            t.equal(error.name, 'Error');
            t.equal(error.message, "[string \"syntax error\"]:1: syntax error near 'error'");

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
    t.test('- compile error', function (t) {
        var scriptpath = temp.path({suffix: '.lua'});
        fs.writeFile(scriptpath, 'syntax error', function (err) {
            t.ifError(err);

            var state = new luajit.LuaState();
            state.doFile(scriptpath, function (error) {
                t.equal(error.name, 'Error');
                t.equal(error.message.slice(-33), ".lua:1: syntax error near 'error'");

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
