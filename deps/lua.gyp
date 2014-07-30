#
# Copyright 2013 Google Inc.
#
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
#

{
    'targets': [
        {
            'target_name': 'lua',
            'type': 'static_library',
            'suppress_wildcard': '1',
            'sources': [
                # core
                'lua/src/lapi.c',
                'lua/src/lcode.c',
                'lua/src/lctype.c',
                'lua/src/ldebug.c',
                'lua/src/ldo.c',
                'lua/src/ldump.c',
                'lua/src/lfunc.c',
                'lua/src/lgc.c',
                'lua/src/llex.c',
                'lua/src/lmem.c',
                'lua/src/lobject.c',
                'lua/src/lopcodes.c',
                'lua/src/lparser.c',
                'lua/src/lstate.c',
                'lua/src/lstring.c',
                'lua/src/ltable.c',
                'lua/src/ltm.c',
                'lua/src/lundump.c',
                'lua/src/lvm.c',
                'lua/src/lzio.c',

                # libraries
                'lua/src/lauxlib.c',
                'lua/src/lbaselib.c',
                'lua/src/lbitlib.c',
                'lua/src/lcorolib.c',
                'lua/src/ldblib.c',
                'lua/src/liolib.c',
                'lua/src/lmathlib.c',
                'lua/src/loslib.c',
                'lua/src/lstrlib.c',
                'lua/src/ltablib.c',
                'lua/src/loadlib.c',
                'lua/src/linit.c'
            ],
            'include_dirs': [
                'lua/src/'
            ],
            'all_dependent_settings': {
                'include_dirs': [
                    'lua/src/'
                ],
            },
            'defines': [
                "getlocaledecpoint()='.'"
            ],
            'conditions': [
                ['OS != "win"', {
                    'defines': [
                        'LUA_USE_POSIX',  # Fix warning re dangerous tmpnam.
                    ],
                }]
            ]
        }
    ]
}
