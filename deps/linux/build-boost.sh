cd src/boost
#build without icu (unicode support needed for regex)
#sudo ./b2 toolset=gcc address-model=64 target-os=linux threading=multi  \
#        --without-mpi --without-python -sNO_BZIP2=1 --layout=tagged --disable-icu variant=release install

sudo ./b2 toolset=gcc address-model=64 target-os=linux threading=multi link=static \
        --without-mpi --without-python -sNO_BZIP2=1 variant=release --prefix=/usr/local/ install

#--layout=tagged    will add the -mt suffix if multithreading
