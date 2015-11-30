export BOOST_ROOT=$PWD/src/boost/
PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent

$BOOST_ROOT/b2 install toolset=clang boost=system address-model=64 boost-link=static link=static \
geoip=off deprecated-functions=off logging=none asserts=off \
cflags="-mmacosx-version-min=10.7" \
cxxflags="-std=c++11 -stdlib=libc++" \
linkflags="-std=c++11 -stdlib=libc++" \
variant=release \
--prefix=$PREFIX_PATH
