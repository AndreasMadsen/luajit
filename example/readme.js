var path = require('path');
var luajit = require('../luajit.js');

var state = new luajit.LuaState();
state.doFile(path.resolve(__dirname, 'readme.lua'), function (err) {
    if (err) throw err;

    state.getGlobal('addition');
    state.push(5);
    state.push(3);
    state.call(2, 1, function (err) {
        if (err) throw err;

        console.log(state.read(-1)); // print 8
        state.pop(1); // lets be nice and cleanup the returned value

        state.close();
    });
});
