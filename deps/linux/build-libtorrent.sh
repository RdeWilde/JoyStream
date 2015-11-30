export BOOST_ROOT=$PWD/src/boost/
PREFIX_PATH=$PWD/dist/release/

cd src/libtorrent

$BOOST_ROOT/b2 install \
  toolset=gcc address-model=64 target-os=linux threading=multi link=static  \
  --without-python boost=system boost-link=static geoip=off deprecated-functions=off logging=none asserts=off variant=release \
  --prefix=$PREFIX_PATH
