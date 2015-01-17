
# Qt Project
TARGET = QtBitSwapr
TEMPLATE = lib

CONFIG  += staticlib
CONFIG  += create_prl # Following http://qt-project.org/doc/qt-5/qmake-advanced-usage.html
CONFIG  += console
CONFIG  += c++11 # Needed for class enum

QT     += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets # Needed for including QApplication

INCLUDEPATH += $$PWD # be able to include w.r.t root of this project

SOURCES += \
    controller/Controller.cpp \
    controller/exceptions/InvalidBitSwaprStateEntryException.cpp \
    controller/exceptions/ListenOnException.cpp \
    controller/exceptions/MissingInfoHashViewRequestException.cpp \
    logger/LoggerManager.cpp \
    logger/exceptions/CannnotOpenLogFile.cpp \
    logger/exceptions/DuplicateLog.cpp \
    view/MainWindow.cpp \
    ControllerTracker.cpp \
    view/AddTorrentDialog.cpp \
    extension/TorrentPlugin.cpp \
    extension/Plugin.cpp \
    extension/PeerPlugin.cpp \
    extension/PeerPluginStatus.cpp \
    extension/Message/ExtendedMessageIdMapping.cpp \
    view/TorrentViewModel.cpp \
    Utilities.cpp \
    view/PeerPluginViewModel.cpp \
    extension/PeerPluginId.cpp \
    controller/ControllerConfiguration.cpp \
    controller/TorrentConfiguration.cpp \
    extension/TorrentPluginConfiguration.cpp \
    extension/PeerPluginConfiguration.cpp \
    extension/Request/PluginRequest.cpp \
    extension/Request/TorrentPluginRequest.cpp \
    extension/Request/PeerPluginRequest.cpp \
    extension/Request/SetConfigurationTorrentPluginRequest.cpp \
    controller/TorrentStatus.cpp \
    extension/Request/SetPluginModeTorrentPluginRequest.cpp \
    view/SellerTorrentPluginConfigurationDialog.cpp \
    view/BuyerTorrentPluginConfigurationDialog.cpp \
    extension/Alert/TorrentPluginAlert.cpp \
    extension/Alert/TorrentPluginStatusAlert.cpp \
    extension/Request/StartPluginTorrentPluginRequest.cpp \
    extension/BitCoin/Client.cpp \
    extension/Alert/PluginStatusAlert.cpp \
    extension/Message/ExtendedMessagePayload.cpp \
    extension/BitCoin/PublicKey.cpp \
    extension/BitCoin/Signature.cpp \
    extension/Message/JoiningContract.cpp \
    extension/Message/SignRefund.cpp \
    extension/BitCoin/Hash.cpp \
    extension/Message/RefundSigned.cpp \
    extension/Message/Buy.cpp \
    extension/Message/Sell.cpp \
    extension/Message/Observe.cpp \
    extension/Message/JoinContract.cpp \
    extension/Message/Payment.cpp \
    extension/Message/End.cpp \
    extension/BitCoin/PrivateKey.cpp \
    extension/PayorPaymentChannel.cpp \
    extension/PaymentChannel/PayorPaymentChannel.cpp \
    extension/PaymentChannel/PayeePaymentChannel.cpp
		
HEADERS += \
    controller/Controller.hpp \
    controller/exceptions/InvalidBitSwaprStateEntryException.hpp \
    controller/exceptions/ListenOnException.hpp \
    controller/exceptions/MissingInfoHashViewRequestException.hpp \
    Config.hpp \
    logger/LoggerManager.hpp \
    logger/exceptions/DuplicateLog.hpp \
    logger/exceptions/CannnotOpenLogFile.hpp \
    view/MainWindow.hpp \
    ControllerTracker.hpp \
    view/AddTorrentDialog.hpp \
    extension/TorrentPlugin.hpp \
    extension/Plugin.hpp \
    extension/PeerPlugin.hpp \
    extension/PeerPluginStatus.hpp \
    extension/Message/ExtendedMessageIdMapping.hpp \
    extension/PeerPluginState.hpp \
    view/TorrentViewModel.hpp \
    Utilities.hpp \
    view/PeerPluginViewModel.hpp \
    extension/Message/MessageType.hpp \
    extension/PeerPluginId.hpp \
    controller/ControllerConfiguration.hpp \
    controller/TorrentConfiguration.hpp \
    extension/TorrentPluginConfiguration.hpp \
    extension/PeerPluginConfiguration.hpp \
    extension/BEPSupportStatus.hpp \
    extension/Request/PluginRequest.hpp \
    extension/Request/TorrentPluginRequest.hpp \
    extension/Request/PeerPluginRequest.hpp \
    extension/Request/TorrentPluginRequestType.hpp \
    extension/Request/PluginRequestType.hpp \
    extension/Request/PeerPluginRequestType.hpp \
    extension/Request/SetConfigurationTorrentPluginRequest.hpp \
    controller/TorrentStatus.hpp \
    extension/Request/SetPluginModeTorrentPluginRequest.hpp \
    view/SellerTorrentPluginConfigurationDialog.hpp \
    view/BuyerTorrentPluginConfigurationDialog.hpp \
    extension/Alert/TorrentPluginAlert.hpp \
    extension/Alert/AlertTypes.hpp \
    extension/Alert/TorrentPluginStatusAlert.hpp \
    extension/PluginMode.hpp \
    extension/Request/StartPluginTorrentPluginRequest.hpp \
    extension/BitCoin/Client.hpp \
    extension/Alert/PluginStatusAlert.hpp \
    extension/Message/ExtendedMessagePayload.hpp \
    extension/BitCoin/PublicKey.hpp \
    extension/BitCoin/Signature.hpp \
    extension/Message/JoiningContract.hpp \
    extension/Message/SignRefund.hpp \
    extension/BitCoin/Hash.hpp \
    extension/Message/RefundSigned.hpp \
    extension/Message/Sell.hpp \
    extension/Message/Observe.hpp \
    extension/Message/JoinContract.hpp \
    extension/Message/Buy.hpp \
    extension/Message/Payment.hpp \
    extension/Message/End.hpp \
    extension/TorrentPluginState.hpp \
    extension/BitCoin/PrivateKey.hpp \
    extension/PayorPaymentChannel.hpp \
    extension/PaymentChannel/PayorPaymentChannel.hpp \
    extension/PaymentChannel/PayeePaymentChannel.hpp
				
FORMS += \
    view/mainwindow.ui \
    view/addtorrentdialog.ui \
    view/SellerTorrentPluginConfigurationDialog.ui \
    view/BuyerTorrentPluginConfigurationDialog.ui

RESOURCES += \
    view/ui.qrc

# Required for including libtorrent and boost headers
include(../defaults.pri)

# Linking with Libtorrent
CONFIG(debug, debug|release) {

    DEFINES += TORRENT_DEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-11.0/debug/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    # The mailinglist suggested this to be able
    LIBS += DbgHelp.lib

    message("Debug Configuration")
}

CONFIG(release, debug|release) {

    DEFINES += NDEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-11.0/release/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    message("Release Configuration")
}

# Linking with boost
LIBS += -L$$BOOST_LOCATION/stage/lib
