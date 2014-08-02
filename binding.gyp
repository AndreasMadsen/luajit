{
    'targets': [
        {
            'target_name': 'bindings',

            # use Lua 5.2 on Mac x64 and otherwise use LuaJIT
            'conditions': [
                ['OS == "mac" and target_arch == "x64"', {
                    'dependencies': [
                        'deps/lua.gyp:lua'
                    ]
                }, {
                    'dependencies': [
                        'deps/luajit.gyp:luajit',
                        'deps/luajit.gyp:libluajit'
                    ]
                }]
            ],

            # Setup source files
            'sources': [
                'src/async.cpp',
                'src/baton.cpp',
                'src/lua_state.cpp',
                'src/lua_utils.cpp',
                'src/luajit.cpp'
            ],
            'include_dirs': [
                'src/',
                "<!(node -e \"require('nan')\")"
            ],

            # Enable c++11 support
            'cflags': ['-std=c++11'],
            'xcode_settings': {
                'OTHER_CFLAGS': ['-std=c++11']
            }
        }
    ]
}
