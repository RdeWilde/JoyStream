/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 24 2017
 */

#include "PluginAlertDecoder.hpp"
#include "../alert.hpp"

#include <extension/extension.hpp>

#define SET_JOYSTREAM_PLUGIN_ALERT_TYPE(o, name) SET_VAL(o, #name, createValue(joystream::extension::alert::name));

namespace joystream {
namespace extension {
namespace node {
namespace PluginAlertEncoder {

  alertEncoder = boost::optional<v8::Local<v8::Object>>(const libtorrent::alert * a) {

    boost::optional<v8::Local<v8::Object>> v;

    if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
        v = encode(p);
    else if(extension::alert::TorrentPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::TorrentPluginStatusUpdateAlert>(a))
        v = encode(p);
    else if(extension::alert::PeerPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::PeerPluginStatusUpdateAlert>(a))
        v = encode(p);
    else if(extension::alert::TorrentPluginAdded const * p = libtorrent::alert_cast<extension::alert::TorrentPluginAdded>(a))
        v = encode(p);
    else if(extension::alert::TorrentPluginRemoved const * p = libtorrent::alert_cast<extension::alert::TorrentPluginRemoved>(a))
        v = encode(p);
    else if(extension::alert::PeerPluginAdded const * p = libtorrent::alert_cast<extension::alert::PeerPluginAdded>(a))
        v = encode(p);
    else if(extension::alert::PeerPluginRemoved const * p = libtorrent::alert_cast<extension::alert::PeerPluginRemoved>(a))
        v = encode(p);
    else if(extension::alert::ConnectionAddedToSession const * p = libtorrent::alert_cast<extension::alert::ConnectionAddedToSession>(a))
        v = encode(p);
    else if(extension::alert::ConnectionRemovedFromSession const * p = libtorrent::alert_cast<extension::alert::ConnectionRemovedFromSession>(a))
        v = encode(p);
    else if(extension::alert::SessionStarted const * p = libtorrent::alert_cast<extension::alert::SessionStarted>(a))
        v = encode(p);
    else if(extension::alert::SessionPaused const * p = libtorrent::alert_cast<extension::alert::SessionPaused>(a))
        v = encode(p);
    else if(extension::alert::SessionStopped const * p = libtorrent::alert_cast<extension::alert::SessionStopped>(a))
        v = encode(p);
    else if(extension::alert::SessionToObserveMode const * p = libtorrent::alert_cast<extension::alert::SessionToObserveMode>(a))
        v = encode(p);
    else if(extension::alert::SessionToSellMode const * p = libtorrent::alert_cast<extension::alert::SessionToSellMode>(a))
        v = encode(p);
    else if(extension::alert::SessionToBuyMode const * p = libtorrent::alert_cast<extension::alert::SessionToBuyMode>(a))
        v = encode(p);
    else if(extension::alert::ValidPaymentReceived const * p = libtorrent::alert_cast<extension::alert::ValidPaymentReceived>(a))
        v = encode(p);
    else if(extension::alert::InvalidPaymentReceived const * p = libtorrent::alert_cast<extension::alert::InvalidPaymentReceived>(a))
        v = encode(p);
    else if(extension::alert::BuyerTermsUpdated const * p = libtorrent::alert_cast<extension::alert::BuyerTermsUpdated>(a))
        v = encode(p);
    else if(extension::alert::SellerTermsUpdated const * p = libtorrent::alert_cast<extension::alert::SellerTermsUpdated>(a))
        v = encode(p);
    else if(extension::alert::ContractConstructed const * p = libtorrent::alert_cast<extension::alert::ContractConstructed>(a))
        v = encode(p);
    else if(extension::alert::SentPayment const * p = libtorrent::alert_cast<extension::alert::SentPayment>(a))
        v = encode(p);
    else if(extension::alert::LastPaymentReceived const * p = libtorrent::alert_cast<extension::alert::LastPaymentReceived>(a))
        v = encode(p);
    else if(extension::alert::InvalidPieceArrived const * p = libtorrent::alert_cast<extension::alert::InvalidPieceArrived>(a))
        v = encode(p);
    else if(extension::alert::ValidPieceArrived const * p = libtorrent::alert_cast<extension::alert::ValidPieceArrived>(a))
        v = encode(p);

    return v;

  }

  NAN_MODULE_INIT(InitAlertTypes) {

    // Export extended alert types
    v8::Local<v8::Object> object = Nan::New<v8::Object>();

    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, RequestResult)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, TorrentPluginStatusUpdateAlert)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, PeerPluginStatusUpdateAlert)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, TorrentPluginAdded)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, TorrentPluginRemoved)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, PeerPluginAdded)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, PeerPluginRemoved)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, ConnectionAddedToSession)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, ConnectionRemovedFromSession)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionStarted)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionPaused)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionStopped)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionToObserveMode)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionToSellMode)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SessionToBuyMode)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, ValidPaymentReceived)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, InvalidPaymentReceived)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, BuyerTermsUpdated)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SellerTermsUpdated)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, ContractConstructed)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, SentPayment)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, LastPaymentReceived)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, InvalidPieceArrived)
    SET_JOYSTREAM_PLUGIN_ALERT_TYPE(object, ValidPieceArrived)

    SET_VAL(target, "AlertType", object);

  }

  v8::Local<v8::Object> encode(extension::alert::RequestResult const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::TorrentPluginStatusUpdateAlert const * p) {
    auto v = libtorrent::node::encode(static_cast<libtorrent::alert const *>(p))

    return v;
  }

  v8::Local<v8::Object> encode(extension::alert::PeerPluginStatusUpdateAlert const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::TorrentPluginAdded const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::TorrentPluginRemoved const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::PeerPluginAdded const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::PeerPluginRemoved const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::ConnectionAddedToSession const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::ConnectionRemovedFromSession const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionStarted const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionPaused const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionStopped const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionToObserveMode const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionToSellMode const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SessionToBuyMode const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::ValidPaymentReceived const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::InvalidPaymentReceived const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::BuyerTermsUpdated const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SellerTermsUpdated const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::ContractConstructed const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::SentPayment const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::LastPaymentReceived const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::InvalidPieceArrived const * p) {

  }

  v8::Local<v8::Object> encode(extension::alert::ValidPieceArrived const * p) {

  }

}
}
}
}
