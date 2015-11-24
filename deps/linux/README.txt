Building Joystream on Linux:

Pre-requisites:

1) Ubuntu + gcc 4.8

    gcc should be upgraded to v4.8 for full C++11 support if using ubuntu <= 13.xx

    https://askubuntu.com/questions/271388/how-to-install-gcc-4-8

2) Install packages
    $ sudo apt-get update && sudo apt-get upgrade
    $ sudo apt-get install ssh git build-essential libssl-dev libsqlite3-dev

3) Download and install Qt 5.5 to your home directory: ~/Qt
   add qmake to the PATH

   edit ubuntu profile:   ~/.profile
   add the following line:

   PATH=~/Qt/5.5/gcc_64/bin:$PATH

4) clone JoyStream repo.

5) Prepare dependencies build folder

    $ cd JoyStream/deps/linux/
    $ mkdir src/
    $ mkdir dist/
   
7) build boost
   download and unzip boost source v1.59 into src/ folder
   rename unzipped boost folder to boost

   compile bjam/b2 build tools in src/boost 
   
   $ ./bootstrap.sh

   run build boost script

   $ cd JoyStream/deps/linux/
   $ ./build-boost.sh

8) build libtorrent

   download and unzip libtorrent source code into JoyStream/deps/linux/src/ folder
   rename unzipped libtorrent folder to libtorrent
   patch libtorrent for 1MB packet limit
   
   build libtorrent:

   $ cd JoyStream/deps/linux
   $ ./build-libtorrent.sh

9) Clone mSIGNA repository into src/ folder:

    $ git clone https://github.com/ciphrex/mSIGNA.git
    
    See instruction below to install ODB

    #todo - write script to only build libCoinCore.a and libCoinQ.a

10) Build qrencode QR Code C library (libqrencode) for linux
    $ cd mSIGNA/deps/qrencode-3.4.3
    $ ./configure --without-tools
    $ make
    $ sudo make install
    $ cd

11) build mSIGNA app and all dependencies

    build mSIGNA ./build-all.sh linux

12) build JoyStream - 

    in JoyStream root folder

    $ ./makelinux-release





Installing ODB
If you want to build msIGNA application and CoinDB
----------------------------------------------------

1) Install ODB compiler and library

    download and install ODB compiler for linux:
    
    http://codesynthesis.com/download/odb/2.4/odb_2.4.0-1_amd64.deb

2) Build ODB Common Runtime Library (libodb) for linux 
    $ wget http://www.codesynthesis.com/download/odb/2.4/libodb-2.4.0.tar.bz2
    $ tar -xjvf libodb-2.4.0.tar.bz2
    $ cd libodb-2.4.0/
    $ ./configure
    $ make
    $ sudo make install
    $ cd ..

3) Build ODB Database Runtime Library for sqlite (libodb-sqlite) for linux 
    $ wget http://www.codesynthesis.com/download/odb/2.4/libodb-sqlite-2.4.0.tar.bz2
    $ tar -xjvf libodb-sqlite-2.4.0.tar.bz2
    $ cd libodb-sqlite-2.4.0/
    $ ./configure
    $ make
    $ sudo make install
    $ cd

