export BOOST_ROOT=$PWD/src/boost/
export PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent/
$PWD/../boost/b2 install toolset=gcc address-model=32 target-os=windows threading=multi threadapi=win32 \
        runtime-link=static link=static --user-config=user-config.jam \
        --without-python boost=system boost-link=static variant=release --prefix=$PREFIX_PATH \
        geoip=off deprecated-functions=off logging=none asserts=off
