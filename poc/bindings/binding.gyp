{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [
                "cpp/NativeExtension.cc",
                "cpp/session.cc",
                "cpp/torrent_handle.cc",
                "cpp/torrent_status.cc",
                "cpp/alert.cc",
                "cpp/torrent_info.cc",
                "cpp/add_torrent_params.cc",
                "cpp/endpoint.cc",
                "../src/Session.cpp"
                ],
            "cflags": ["-Wall", "-std=c++11", "-fPIC", '-frtti'],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                "../include/",
                "./qt_headers/",
                "../../common/include",
                "../../protocol_session/include",
                "../../protocol_statemachine/include",
                "../../protocol_wire/include",
                "../../paymentchannel/include",
                "../../extension/include",
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
                    "-lextension",
                    "-lpaymentchannel",
                    "-lprotocol_session",
                    "-lprotocol_statemachine",
                    "-lprotocol_wire",
                    "-ltorrent",
                    "-lcommon",
                    "-lCoinCore",
                    # we need to link to openssl for electron (electron executable
                    # does not export openssl symbols) - nodejs exports openssl symbols
                    # we should ensure we use the headers for the same version
                    "-lcrypto",
                    "-lssl",
                ]
            },
            "conditions": [
                [ 'OS=="mac"', {
                    "xcode_settings": {
                        'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                        'OTHER_LDFLAGS': ['-stdlib=libc++', "-F<!(qmake -query QT_INSTALL_LIBS)/"],
                        'MACOSX_DEPLOYMENT_TARGET': '10.7',
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                        'GCC_ENABLE_CPP_RTTI': '-frtti',
                        'LD_RUNPATH_SEARCH_PATHS': [
                             "<!(qmake -query QT_INSTALL_LIBS)/",
                             "@loader_path/../../Frameworks/"
                        ],
                    },
                    "link_settings": {
                        "libraries": [
                            "-lboost_chrono",
                            "-lboost_random",
                            "-lboost_system",
                            "QtCore.framework"
                        ]
                     },
                     'library_dirs': [
                        '${PWD}/../../build/osx/common/',
                        '${PWD}/../../build/osx/protocol_session/',
                        '${PWD}/../../build/osx/protocol_wire/',
                        '${PWD}/../../build/osx/protocol_statemachine/',
                        '${PWD}/../../build/osx/paymentchannel/',
                        '${PWD}/../../build/osx/extension/',
                        '${PWD}/../../deps/osx/dist/release/lib/',
                        "${PWD}/../../deps/osx/src/mSIGNA-joystream/sysroot/lib/",
                        '/usr/local/lib',
                     ],
                     "include_dirs" : [
                         "../../deps/osx/dist/release/include/",
                         "<!(qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/",
                         "../../deps/osx/src/mSIGNA-joystream/sysroot/include/",
                         "/usr/local/include/",
                     ],
                     "postbuilds": [ {
                       'postbuild_name': 'Reconfigure @rpath',
                       'action': [
                         #"${PWD}/post_build.sh", "${BUILT_PRODUCTS_DIR}/NativeExtension.node"
                         "${PWD}/copy_qt_framework.sh"
                       ],
                     }]
                }],
                [ 'OS=="linux"', {
                    "link_settings": {
                        "libraries": [
                            "-lboost_chrono-mt",
                            "-lboost_random-mt",
                            "-lboost_system-mt",
                            "-lQt5Core"
                        ]
                    },
                    'library_dirs': [
                       '${PWD}/../../build/linux-x64/common/',
                       '${PWD}/../../build/linux-x64/protocol_session/',
                       '${PWD}/../../build/linux-x64/protocol_wire/',
                       '${PWD}/../../build/linux-x64/protocol_statemachine/',
                       '${PWD}/../../build/linux-x64/paymentchannel/',
                       '${PWD}/../../build/linux-x64/extension/',
                       '${PWD}/../../deps/linux/dist/release/lib/',
                       '<!(qmake -query QT_INSTALL_LIBS)/',
                       "${PWD}/../../deps/linux/src/mSIGNA-joystream/sysroot/lib/",
                       '/usr/local/lib',
                    ],
                    "include_dirs" : [
                        "../../deps/linux/dist/release/include/",
                        "<!(qmake -query QT_INSTALL_HEADERS)/QtCore/",
                        "../../deps/linux/src/mSIGNA-joystream/sysroot/include/",
                        "/usr/local/include/",
                    ],
                }]
            ],
        },
    ]
}
