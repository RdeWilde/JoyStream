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
                "libtorrent-node/session.cpp",
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
                "joystream-node/Common.cpp",
                "joystream-node/StartDownloadConnectionInformation.cpp",
                "joystream-node/PluginAlertEncoder.cpp",
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
                "joystream-node/Signature.cpp",
                "joystream-node/Connection.cpp",
                "joystream-node/payment_channel.cpp",
                "joystream-node/Session.cpp",
                "joystream-node/BEPSupportStatus.cpp",
                "joystream-node/buffers.cpp"
                # Others
                ],
            "cflags": ["-Wall", "-std=c++11", "-fPIC", '-frtti'],
            'cflags!': [ '-fno-exceptions', '-fno-rtti' ],
            'cflags_cc!': [ '-fno-exceptions', '-fno-rtti' ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                ".", # include w.r.t. top level folder, primarily for joystream-node for the time being
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
                #'TORRENT_LINKING_SHARED',
                'TORRENT_DISABLE_LOGGING',
                'TORRENT_USE_ASSERTS=0',
                'NDEBUG', #make sure to link against release builds
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
                        'OTHER_LDFLAGS': ['-stdlib=libc++'],
                        'MACOSX_DEPLOYMENT_TARGET': '10.7',
                        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                        'GCC_ENABLE_CPP_RTTI': '-frtti',
                    },
                    "link_settings": {
                        "libraries": [
                            "-lboost_chrono",
                            "-lboost_random",
                            "-lboost_system",
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
                        '/usr/local/lib', #openssl, boost
                     ],
                     "include_dirs" : [
                         "../../deps/osx/dist/release/include/",
                         "../../deps/osx/src/mSIGNA-joystream/sysroot/include/",
                         "/usr/local/include/", #openssl, boost
                     ]
                }],
                [ 'OS=="linux"', {
                    "link_settings": {
                        "libraries": [
                            "-lboost_chrono-mt",
                            "-lboost_random-mt",
                            "-lboost_system-mt",
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
                       "$(srcdir)/../../deps/linux/src/mSIGNA-joystream/sysroot/lib/",
                       '/usr/local/lib', #openssl
                    ],
                    "include_dirs" : [
                        "../../deps/linux/dist/release/include/",
                        "../../deps/linux/src/mSIGNA-joystream/sysroot/include/",
                        "/usr/local/include/", #openssl
                    ]
                }]
            ],
        },
    ]
}
