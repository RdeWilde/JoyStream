{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [ "cpp/NativeExtension.cc", "cpp/functions.cc", "../src/Session.cpp" ],
            "cflags": ["-Wall", "-std=c++11"],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")",
                "../include/",
                "../../deps/osx/dist/release/include/",
                "<!(qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/",
                "./qt_headers/"
            ],
            'defines': [
                'TORRENT_DISABLE_GEO_IP',
                'TORRENT_NO_DEPRECATE',
                'TORRENT_LINKING_STATIC',
                'NDEBUG',
                'TORRENT_DISABLE_LOGGING',
                'TORRENT_USE_ASSERTS=0'
            ],
            "conditions": [
                [ 'OS=="mac"', {
                    "xcode_settings": {
                        'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                        'OTHER_LDFLAGS': ['-stdlib=libc++', "-L../../deps/osx/dist/release/lib/", "-F<!(qmake -query QT_INSTALL_LIBS)/"],
                        'MACOSX_DEPLOYMENT_TARGET': '10.7',
                        # 'LD_RUNPATH_SEARCH_PATHS': [
                        #     "./Frameworks"
                        # ],
                    },
                    "link_settings": {
                        "libraries": [
                            "-ltorrent",
                            "-lboost_system",
                            "-lboost_random",
                            "-lcrypto",
                            "-lssl",
                            "QtCore.framework"
                        ]
                     }
                  }
                ]
            ]
        }
    ],
}
