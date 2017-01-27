{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [
                # base level module binding (this is temporary)
                "NativeExtension.cpp",
                # libtorrent
                "libtorrent-node/init.cpp",
                "libtorrent-node/alert.cpp",
                "libtorrent-node/sha1_hash.cpp",
                "libtorrent-node/session.cc",
                "libtorrent-node/torrent_handle.cc",
                "libtorrent-node/torrent_status.cpp",
                "libtorrent-node/torrent_info.cc",
                "libtorrent-node/add_torrent_params.cpp",
                "libtorrent-node/endpoint.cpp",
                "libtorrent-node/peer_info.cpp",
                "libtorrent-node/bencode.cc",
                "libtorrent-node/state_t.cpp",
                "libtorrent-node/entry.cpp",
                "libtorrent-node/error_code.cpp",
                "libtorrent-node/utils.cpp",
                # joystream
                "joystream-node/Init.cpp",
                "joystream-node/Plugin.cpp",
                "joystream-node/RequestResult.cpp",
                "joystream-node/TorrentPluginStatus.cpp",
                "joystream-node/PeerPluginStatus.cpp",
                "joystream-node/BuyerTerms.cpp",
                "joystream-node/SellerTerms.cpp",
                "joystream-node/LibtorrentInteraction.cpp",
                "joystream-node/Transaction.cpp",
                "joystream-node/PrivateKey.cpp",
                "joystream-node/PubKeyHash.cpp",
                "joystream-node/PublicKey.cpp",
                "joystream-node/OutPoint.cpp",
                "joystream-node/TransactionId.cpp",
                "joystream-node/Connection.cpp",
                "joystream-node/buffers.cpp",
                # Others
                "../src/Session.cpp"
                ],
            "cflags": ["-Wall", "-std=c++11", "-fPIC", '-frtti'],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                "../include/",
                ".", # include w.r.t. top level folder, primarily for joystream-node for the time being
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
                        '$(srcdir)/../../build/osx/common/',
                        '$(srcdir)/../../build/osx/protocol_session/',
                        '$(srcdir)/../../build/osx/protocol_wire/',
                        '$(srcdir)/../../build/osx/protocol_statemachine/',
                        '$(srcdir)/../../build/osx/paymentchannel/',
                        '$(srcdir)/../../build/osx/extension/',
                        '$(srcdir)/../../deps/osx/dist/release/lib/',
                        "$(srcdir)/../../deps/osx/src/mSIGNA-joystream/sysroot/lib/",
                        '/usr/local/lib',
                     ],
                     "include_dirs" : [
                         "../../deps/osx/dist/release/include/",
                         "<!(qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/",
                         "../../deps/osx/src/mSIGNA-joystream/sysroot/include/",
                         "/usr/local/include/",
                     ]
                }],
                [ 'OS=="linux"', {
                    "link_settings": {
                        "ldflags" : [ '-Wl,-rpath=XORIGIN/../../Frameworks' ],
                        "libraries": [
                            "-lboost_chrono-mt",
                            "-lboost_random-mt",
                            "-lboost_system-mt",
                            "-lQt5Core"
                        ]
                    },
                    'library_dirs': [
                       "$(srcdir)/../../build/linux-x64/common/",
                       '$(srcdir)/../../build/linux-x64/protocol_session/',
                       '$(srcdir)/../../build/linux-x64/protocol_wire/',
                       '$(srcdir)/../../build/linux-x64/protocol_statemachine/',
                       '$(srcdir)/../../build/linux-x64/paymentchannel/',
                       '$(srcdir)/../../build/linux-x64/extension/',
                       '$(srcdir)/../../deps/linux/dist/release/lib/',
                       '<!(qmake -query QT_INSTALL_LIBS)/',
                       "$(srcdir)/../../deps/linux/src/mSIGNA-joystream/sysroot/lib/",
                       '/usr/local/lib',
                    ],
                    "include_dirs" : [
                        "../../deps/linux/dist/release/include/",
                        "<!(qmake -query QT_INSTALL_HEADERS)/QtCore/",
                        "../../deps/linux/src/mSIGNA-joystream/sysroot/include/",
                        "/usr/local/include/",
                    ]
                }]
            ],
        },
    ]
}
