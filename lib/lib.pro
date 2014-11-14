include(../defaults.pri)

TARGET = QtBitSwapr
TEMPLATE = lib
CONFIG += staticlib

# Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG += create_prl

SOURCES += \
            controller/Controller.cpp \
            controller/ControllerState.cpp \
            controller/Exceptions/InvalidBitSwaprStateEntryException.cpp \
            controller/Exceptions/ListenOnException.cpp \
            controller/Exceptions/MissingInfoHashViewRequestException.cpp \
            view/mainwindow.cpp \
            view/addtorrentdialog.cpp \
    extension/BitSwaprPlugin.cpp \
    extension/BitSwaprTorrentPlugin.cpp \
    extension/BitSwaprPeerPlugin.cpp
		
HEADERS += \
            controller/Controller.hpp \
            controller/ControllerState.hpp \
            controller/Exceptions/InvalidBitSwaprStateEntryException.hpp \
            controller/Exceptions/ListenOnException.hpp \
            controller/Exceptions/MissingInfoHashViewRequestException.hpp \
            view/mainwindow.hpp \
            view/addtorrentdialog.hpp \
            Config.hpp \
    extension/BitSwaprPlugin.hpp \
    extension/BitSwaprTorrentPlugin.hpp \
    extension/BitSwaprPeerPlugin.hpp
				
FORMS += \
            view/mainwindow.ui \
            view/addtorrentdialog.ui

OTHER_FILES += \
            resources/BitSwapr_mark_32.png
