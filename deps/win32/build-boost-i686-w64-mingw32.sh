cd src/boost
sudo ./b2 toolset=gcc address-model=32 target-os=windows threading=multi threadapi=win32 \
        link=static runtime-link=static --prefix=/usr/i686-w64-mingw32 --user-config=user-config.jam \
        --without-mpi --without-python -sNO_BZIP2=1 --layout=tagged variant=release install
