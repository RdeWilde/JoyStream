include(../defaults.pri)

TARGET = QtBitSwapr
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
            controller/Controller.cpp \
            controller/ControllerState.cpp \
            controller/Exceptions/InvalidBitSwaprStateEntryException.cpp \
            controller/Exceptions/ListenOnException.cpp \
            controller/Exceptions/MissingInfoHashViewRequestException.cpp \
            extension/BrPayment.cpp \
            view/mainwindow.cpp \
            view/addtorrentdialog.cpp
		
HEADERS += \
            controller/Controller.hpp \
            controller/ControllerState.hpp \
            controller/Exceptions/InvalidBitSwaprStateEntryException.hpp \
            controller/Exceptions/ListenOnException.hpp \
            controller/Exceptions/MissingInfoHashViewRequestException.hpp \
            extension/BrPayment.hpp \
            view/mainwindow.hpp \
            view/addtorrentdialog.hpp \
            Config.hpp
				
FORMS += \
            view/mainwindow.ui \
            view/addtorrentdialog.ui

OTHER_FILES += \
            resources/BitSwapr_mark_32.png