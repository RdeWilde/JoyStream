include(../defaults.pri)

TARGET = QtBitSwapr
TEMPLATE = lib
CONFIG += staticlib

# Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG += create_prl

SOURCES += \
            controller/Controller.cpp \
            controller/ControllerState.cpp \
            controller/exceptions/InvalidBitSwaprStateEntryException.cpp \
            controller/exceptions/ListenOnException.cpp \
            controller/exceptions/MissingInfoHashViewRequestException.cpp \
            view/mainwindow.cpp \
            view/addtorrentdialog.cpp \
            logger/CategoryLoggerManager.cpp \
            logger/exceptions/DuplicateCategoryLog.cpp \
            logger/exceptions/CannnotOpenCategoryLogFile.cpp \
            extension/BitSwaprPlugin.cpp \
            extension/BitSwaprTorrentPlugin.cpp \
            extension/BitSwaprPeerPlugin.cpp \
            BitSwapr.cpp \
		
HEADERS += \
            controller/Controller.hpp \
            controller/ControllerState.hpp \
            controller/exceptions/InvalidBitSwaprStateEntryException.hpp \
            controller/exceptions/ListenOnException.hpp \
            controller/exceptions/MissingInfoHashViewRequestException.hpp \
            view/mainwindow.hpp \
            view/addtorrentdialog.hpp \
            logger/CategoryLoggerManager.hpp \
            logger/exceptions/DuplicateCategoryLog.hpp \
            logger/exceptions/CannnotOpenCategoryLogFile.hpp \
            extension/BitSwaprPlugin.hpp \
            extension/BitSwaprTorrentPlugin.hpp \
            extension/BitSwaprPeerPlugin.hpp \
            Config.hpp \
            BitSwapr.hpp
				
FORMS += \
            view/mainwindow.ui \
            view/addtorrentdialog.ui

OTHER_FILES += \
            resources/BitSwapr_mark_32.png
