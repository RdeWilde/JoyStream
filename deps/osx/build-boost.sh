cd src/boost
./b2 toolset=clang address-model=64 link=static \
cflags=-mmacosx-version-min=10.7 \
cxxflags="-std=c++11 -stdlib=libc++" \
linkflags="-std=c++11 -stdlib=libc++" \
macosx-version=10.11 \
macosx-version-min=10.7 \
install


#remove shared boost libs (keep only static libs)
#rm /usr/local/lib/libboost_*.dylib
