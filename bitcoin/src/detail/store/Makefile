
#determine target platform with qmake (update this in future to use cmake
#to find the location of mSIGNA headers (CoinQ, CoinCore, stdutils)
SPEC=$(shell qmake -query QMAKE_XSPEC)

#OSX
ifeq ($(SPEC), macx-clang)
  INCLUDES+=-I../../../../deps/osx/src/mSIGNA-joystream/sysroot/include
endif

#linux
ifeq ($(SPEC), linux-g++)
  INCLUDES+=-I../../../../deps/linux/src/mSIGNA-joystream/sysroot/include
endif

#win32
ifeq ($(SPEC), win32-g++)
  INCLUDES+=-I../../../../deps/win32/src/mSIGNA-joystream/sysroot/include
endif


#joystream common lib headers
INCLUDES+=-I../../../../common/include

# on linux we need to compile with position independent code using "-x -fPIE" flags
# http://www.codesynthesis.com/pipermail/odb-users/2015-June/002630.html

Schema-odb.cxx: Schema.hpp
	odb --std c++11 -d sqlite -x -fPIE \
        --generate-query --generate-schema --schema-format embedded $(INCLUDES) Schema.hpp
clean:
	rm *-odb.cxx
	rm *-odb.hxx
	rm *-odb.ixx

# Do not delete Schema.xml !
