#store depends on joystream common lib which still depends on Qt so we need
#to access to the Qt headers during build
#since we are not using qmake .. a bit of hacking...
#for including Qt headers using this pattern:  #include <QtCore/QString>
$(shell sh ./config-qt-headers.sh)
INCLUDES=-I./qt_headers/

#for including headers using this pattern:   #include <QString>  (without QtCore/ prefix)
INCLUDES+=-I$(shell qmake -query QT_INSTALL_LIBS)/QtCore.framework/Headers/


INCLUDES+=-I../../../../common/include
#assuming you are on either osx or linux (doesn't really matter
#as long as using the same version of odb compiler will produce the
#the same source code)
INCLUDES+=-I../../../../deps/osx/src/mSIGNA/sysroot/include
INCLUDES+=-I../../../../deps/linux/src/mSIGNA/sysroot/include

Schema-odb.cxx: Schema.hpp
	odb --std c++11 -d sqlite \
        --generate-query --generate-schema --schema-format embedded $(INCLUDES) Schema.hpp
clean:
	rm *-odb.cxx
	rm *-odb.hxx
	rm *-odb.ixx

#do not delete Schema.xml
