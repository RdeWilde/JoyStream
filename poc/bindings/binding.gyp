{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [ "cpp/NativeExtension.cc", "cpp/functions.cc", "../src/Session.cpp" ],
            "cflags": ["-Wall", "-std=c++11"],
            'cflags!': [ '-fno-exceptions' ],
            'cflags_cc!': [ '-fno-exceptions' ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                "../include/",
                "../../deps/osx/dist/release/include/",
                "/usr/local/include/",
                "<!(qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/",
                "./qt_headers/"
            ],
            'defines': [
                'TORRENT_DISABLE_GEO_IP',
                'TORRENT_NO_DEPRECATE',
                'TORRENT_LINKING_STATIC',
                'NDEBUG',
                'TORRENT_DISABLE_LOGGING',
                'TORRENT_USE_ASSERTS=0',
                'BOOST_ASIO_SEPARATE_COMPILATION'
            ],
            "link_settings": {
                "libraries": [
                    "-ltorrent",
                    "-lboost_system",
                    "-lboost_random",
                    # we don't need them for node.js (might be required for electron build?)
                    #"-lcrypto",
                    #"-lssl",
                ]
            },
            "conditions": [
                [ 'OS=="mac"', {
                    "xcode_settings": {
                        'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                        'OTHER_LDFLAGS': ['-stdlib=libc++', "-L../../deps/osx/dist/release/lib/", "-F<!(qmake -query QT_INSTALL_LIBS)/"],
                        'MACOSX_DEPLOYMENT_TARGET': '10.7',
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                        # 'LD_RUNPATH_SEARCH_PATHS': [
                        #     "./Frameworks"
                        # ],
                    },
                    "link_settings": {
                        "libraries": [
                            "QtCore.framework"
                        ]
                     },
                     "postbuilds": [ {
                       'postbuild_name': 'Reconfigure @rpath',
                       'action': [
                         "${PWD}/post_build.sh", "${BUILT_PRODUCTS_DIR}/NativeExtension.node"
                       ],
                     }]
                }],
                [ 'OS=="linux"', {
                    "link_settings": {
                        "libraries": [
                            "-lQtCore"
                        ]
                    }
                }]
            ],
        },
    ]
}
