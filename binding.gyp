{
    'targets': [
        {
            'target_name': 'bindings',
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
            'sources': [
                'src/lua_state.cpp',
                'src/luajit.cpp'
            ],
            'include_dirs': [
                'src/',
                "<!(node -e \"require('nan')\")"
            ]
        }
    ]
}
