
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
    extension/Message/ExtendedMessageIdMapping.cpp \
    Utilities.cpp \
    extension/PeerPluginId.cpp \
    extension/Request/PluginRequest.cpp \
    extension/Request/TorrentPluginRequest.cpp \
    extension/Request/PeerPluginRequest.cpp \
    #extension/Request/SetConfigurationTorrentPluginRequest.cpp \
    #extension/Request/SetPluginModeTorrentPluginRequest.cpp \
    view/SellerTorrentPluginConfigurationDialog.cpp \
    view/BuyerTorrentPluginConfigurationDialog.cpp \
    extension/Alert/TorrentPluginAlert.cpp \
    extension/BitCoin/Client.cpp \
    extension/Alert/PluginStatusAlert.cpp \
    extension/Message/ExtendedMessagePayload.cpp \
    extension/BitCoin/Signature.cpp \
    extension/Message/JoiningContract.cpp \
    extension/Message/SignRefund.cpp \
    extension/Message/RefundSigned.cpp \
    extension/Message/Buy.cpp \
    extension/Message/Sell.cpp \
    extension/Message/Observe.cpp \
    extension/Message/JoinContract.cpp \
    extension/Message/Payment.cpp \
    extension/Message/Ready.cpp \
    extension/Message/NoPayloadMessage.cpp \
    extension/BuyerTorrentPlugin.cpp \
    extension/SellerTorrentPlugin.cpp \
    extension/SellerPeerPlugin.cpp \
    extension/BitCoin/KeyPair.cpp \
    extension/BitCoin/P2SHTxOut.cpp \
    extension/BitCoin/P2PKHTxOut.cpp \
    #extension/PaymentChannel/Refund.cpp \
    #extension/PaymentChannel/Payment.cpp \
    #extension/PaymentChannel/Contract.cpp \
    extension/BitCoin/BitSwaprjs.cpp \
    #extension/PaymentChannel/Payee/Payee.cpp \
    #extension/PaymentChannel/Payor/Channel.cpp \
    extension/BitCoin/OutPoint.cpp \
    extension/BuyerPeerPlugin.cpp \
    #extension/PaymentChannel/Payor/Payor.cpp \
    extension/PaymentChannel/Payor.cpp \
    extension/PaymentChannel/Payee.cpp \
    extension/BitCoin/UnspentP2PKHOutput.cpp \
    extension/BitCoin/Wallet.cpp \
    view/WalletDialog.cpp \
    extension/BitCoin/PublicKey.cpp \
    extension/BitCoin/TxId.cpp \
    extension/BitCoin/base58.cpp \
    extension/BitCoin/PrivateKey.cpp \
    extension/Request/StartBuyerTorrentPlugin.cpp \
    extension/Request/StartSellerTorrentPlugin.cpp \
    extension/Request/StartObserverTorrentPlugin.cpp \
    extension/Message/RequestFullPiece.cpp \
    extension/Message/FullPiece.cpp \
    extension/Alert/BuyerTorrentPluginStatusAlert.cpp \
    extension/Alert/TorrentPluginStartedAlert.cpp \
    view/PeerPluginsDialog.cpp \
    extension/Alert/SellerTorrentPluginStatusAlert.cpp \
    view/SellerTorrentPluginDialog.cpp \
    extension/Alert/StartedSellerTorrentPlugin.cpp \
    extension/Alert/StartedBuyerTorrentPlugin.cpp \
    view/BuyerTorrentPluginDialog.cpp \
    controller/BuyerPeerPluginViewModel.cpp \
    controller/BuyerTorrentPluginViewModel.cpp \
    controller/ChannelViewModel.cpp \
    controller/PayeeViewModel.cpp \
    controller/PayorViewModel.cpp \
    controller/PeerPluginViewModel.cpp \
    controller/SellerPeerPluginViewModel.cpp \
    controller/SellerTorrentPluginViewModel.cpp \
    controller/TorrentPluginViewModel.cpp \
    controller/TorrentViewModel.cpp \
    view/TorrentView.cpp \
    view/ChannelView.cpp \
    view/BuyerPeerPluginView.cpp \
    view/SellerPeerPluginView.cpp \
    extension/Alert/BuyerPeerAddedAlert.cpp \
    extension/Alert/SellerPeerAddedAlert.cpp \
    extension/Alert/BuyerPeerPluginRemovedAlert.cpp \
    extension/Alert/SellerPeerPluginRemovedAlert.cpp
		
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
    extension/Message/ExtendedMessageIdMapping.hpp \
    Utilities.hpp \
    extension/Message/MessageType.hpp \
    extension/PeerPluginId.hpp \
    extension/BEPSupportStatus.hpp \
    extension/Request/PluginRequest.hpp \
    extension/Request/TorrentPluginRequest.hpp \
    extension/Request/PeerPluginRequest.hpp \
    extension/Request/TorrentPluginRequestType.hpp \
    extension/Request/PluginRequestType.hpp \
    extension/Request/PeerPluginRequestType.hpp \
    extension/Request/SetPluginModeTorrentPluginRequest.hpp \
    view/SellerTorrentPluginConfigurationDialog.hpp \
    view/BuyerTorrentPluginConfigurationDialog.hpp \
    extension/Alert/TorrentPluginAlert.hpp \
    extension/Alert/AlertTypes.hpp \
    extension/PluginMode.hpp \
    extension/BitCoin/Client.hpp \
    extension/Alert/PluginStatusAlert.hpp \
    extension/Message/ExtendedMessagePayload.hpp \
    extension/BitCoin/PublicKey.hpp \
    extension/BitCoin/Signature.hpp \
    extension/Message/JoiningContract.hpp \
    extension/Message/SignRefund.hpp \
    extension/Message/RefundSigned.hpp \
    extension/Message/Sell.hpp \
    extension/Message/Observe.hpp \
    extension/Message/JoinContract.hpp \
    extension/Message/Buy.hpp \
    extension/Message/Payment.hpp \
    extension/BitCoin/PrivateKey.hpp \
    extension/PayorPaymentChannel.hpp \
    extension/PeerAction.hpp \
    extension/Message/Ready.hpp \
    extension/Message/NoPayloadMessage.hpp \
    extension/SellModePeerAction.hpp \
    BuyModePeerAction.hpp \
    extension/BuyModePeerAction.hpp \
    extension/BuyerTorrentPlugin.hpp \
    extension/SellerTorrentPlugin.hpp \
    extension/BuyerPeerPlugin.hpp \
    extension/SellerPeerPlugin.hpp \
    extension/BitCoin/KeyPair.hpp \
    extension/BitCoin/FixedBuffer.hpp \
    extension/BitCoin/OutputSigHash.hpp \
    extension/BitCoin/P2SHTxOut.hpp \
    extension/BitCoin/P2PKHTxOut.hpp \
    extension/PaymentChannel/Refund.hpp \
    extension/PaymentChannel/Contract.hpp \
    extension/BitCoin/BitSwaprjs.hpp \
    extension/PaymentChannel/Payee.hpp \
    extension/BitCoin/P2PKHAddress.hpp \
    extension/BitCoin/OutPoint.hpp \
    extension/PaymentChannel/Payor.hpp \
    extension/BitCoin/UnspentP2PKHOutput.hpp \
    extension/BitCoin/Wallet.hpp \
    view/WalletDialog.hpp \
    extension/BitCoin/TxId.hpp \
    extension/BitCoin/base58.hpp \
    extension/Request/StartBuyerTorrentPlugin.hpp \
    extension/Request/StartSellerTorrentPlugin.hpp \
    extension/Request/StartObserverTorrentPlugin.hpp \
    extension/Message/RequestFullPiece.hpp \
    extension/Message/FullPiece.hpp \
    extension/BitCoin/BitCoin.hpp \
    extension/Alert/BuyerTorrentPluginStatusAlert.hpp \
    extension/Alert/TorrentPluginStartedAlert.hpp \
    view/PeerPluginsDialog.hpp \
    controller/PluginInstalled.hpp \
    extension/Alert/SellerTorrentPluginStatusAlert.hpp \
    view/SellerTorrentPluginDialog.hpp \
    extension/Alert/StartedSellerTorrentPlugin.hpp \
    extension/Alert/StartedBuyerTorrentPlugin.hpp \
    view/BuyerTorrentPluginDialog.hpp \
    controller/BuyerPeerPluginViewModel.hpp \
    controller/BuyerTorrentPluginViewModel.hpp \
    controller/ChannelViewModel.hpp \
    controller/PayeeViewModel.hpp \
    controller/PayorViewModel.hpp \
    controller/PeerPluginViewModel.hpp \
    controller/SellerPeerPluginViewModel.hpp \
    controller/SellerTorrentPluginViewModel.hpp \
    controller/TorrentPluginViewModel.hpp \
    controller/TorrentViewModel.hpp \
    view/TorrentView.hpp \
    view/ChannelView.hpp \
    view/BuyerPeerPluginView.hpp \
    view/SellerPeerPluginView.hpp \
    extension/Alert/BuyerPeerAddedAlert.hpp \
    extension/Alert/SellerPeerAddedAlert.hpp \
    extension/Alert/BuyerPeerPluginRemovedAlert.hpp \
    extension/Alert/SellerPeerPluginRemovedAlert.hpp
				
FORMS += \
    view/SellerTorrentPluginConfigurationDialog.ui \
    view/BuyerTorrentPluginConfigurationDialog.ui \
    view/PeerPluginsDialognsdialog.ui \
    view/SellerTorrentPluginDialog.ui \
    view/BuyerTorrentPluginDialog.ui \
    view/MainWindow.ui \
    view/AddTorrentDialog.ui \
    view/WalletDialog.ui

RESOURCES += \
    ../views/gui/src/resources.qrc

# Required for including libtorrent and boost headers
include(../defaults.pri)

####################################
# Linking with Libtorrent RELEASE
####################################
CONFIG(release, debug|release) {

    DEFINES += NDEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/release/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    message("Release Configuration")
}

####################################
# Linking with Libtorrent DEBUG
####################################
CONFIG(debug, debug|release) {

    DEFINES += TORRENT_DEBUG

    LIBS += -L$$LIBTORRENT_LOCATION/bin/msvc-12.0/debug/address-model-64/boost-source/deprecated-functions-off/link-static/threading-multi -llibtorrent

    # The mailinglist suggested this to be able
    LIBS += DbgHelp.lib

    message("Debug Configuration")
}

# Linking with boost
LIBS += -L$$BOOST_LOCATION/stage/lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../streamingserver/streamingserver-lib/release/ -lstreamingserver-lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../streamingserver/streamingserver-lib/debug/ -lstreamingserver-lib
else:unix: LIBS += -L$$OUT_PWD/../streamingserver/streamingserver-lib/ -lstreamingserver-lib

INCLUDEPATH += $$PWD/../streamingserver/streamingserver-lib/include
DEPENDPATH += $$PWD/../streamingserver/streamingserver-lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver/streamingserver-lib/release/libstreamingserver-lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver/streamingserver-lib/debug/libstreamingserver-lib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver/streamingserver-lib/release/streamingserver-lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../streamingserver/streamingserver-lib/debug/streamingserver-lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../streamingserver/streamingserver-lib/libstreamingserver-lib.a
