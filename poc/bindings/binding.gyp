{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [
                "cpp/NativeExtension.cc",
                # Libtorrent
				"cpp/alert.cpp",
                "cpp/info_hash.cpp",
                "cpp/session.cc",
                "cpp/torrent_handle.cc",
                "cpp/torrent_status.cpp",
                "cpp/torrent_info.cc",
                "cpp/add_torrent_params.cpp",
                "cpp/endpoint.cpp",
                "cpp/peer_info.cpp",
                "cpp/bencode.cc",
                "cpp/state_t.cpp",
                "cpp/entry.cpp",
                "cpp/error_code.cpp",
                "cpp/utils.cpp",
                # Extension
                "cpp/joystream-node/TorrentPluginStatus.cpp",
                "cpp/joystream-node/PeerPluginStatus.cpp",
                "cpp/joystream-node/BuyerTerms.cpp",
                "cpp/joystream-node/SellerTerms.cpp",
                "cpp/joystream-node/LibtorrentInteraction.cpp",
                "cpp/joystream-node/Transaction.cpp",
                "cpp/joystream-node/PrivateKey.cpp",
                "cpp/joystream-node/PubKeyHash.cpp",
                "cpp/joystream-node/PublicKey.cpp",
                "cpp/joystream-node/OutPoint.cpp",
                "cpp/joystream-node/TransactionId.cpp",
                "cpp/joystream-node/Connection.cpp",
                "cpp/joystream-node/buffers.cpp",
                # Others
                "../src/Session.cpp"
                ],
            "cflags": ["-Wall", "-std=c++11", "-fPIC", '-frtti'],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                "cpp/include/",
                "cpp/",
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
