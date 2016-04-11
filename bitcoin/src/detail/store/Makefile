#store depends on joystream common lib which still depends on Qt so we need
#to access to the Qt Core headers during build
#since we are not using qmake .. a bit of hacking...

#determine target platform from qmake
SPEC=$(shell qmake -query QMAKE_XSPEC)

#for including Qt headers using this pattern: #include <QtCore/QString>
$(shell sh ./config-qt-headers.sh)
INCLUDES=-I./qt_headers/


#for including headers using this pattern:    #include <QString>  (without QtCore/ prefix)

#OSX
ifeq ($(SPEC), macx-clang)
  INCLUDES+=-I$(shell qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/
  INCLUDES+=-I../../../../deps/osx/src/mSIGNA/sysroot/include
endif

#linux
ifeq ($(SPEC), linux-g++)
  INCLUDES+=-I$(shell qmake -query QT_INSTALL_HEADERS)/QtCore
  INCLUDES+=-I../../../../deps/linux/src/mSIGNA/sysroot/include
endif

#win32
ifeq ($(SPEC), win32-g++)
  INCLUDES+=-I$(shell qmake -query QT_INSTALL_HEADERS)/QtCore
  INCLUDES+=-I../../../../deps/win32/src/mSIGNA/sysroot/include
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
