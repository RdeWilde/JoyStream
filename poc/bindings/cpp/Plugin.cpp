/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 12 2017
 */

#include "plugin.hpp"

#include "plugin_alert_to_object_converter.hpp"

namespace joystream {
namespace node_addon {

NAN_MODULE_INIT(plugin::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("Plugin").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "start", Start);
  Nan::SetPrototypeMethod(tpl, "stop", Stop);
  Nan::SetPrototypeMethod(tpl, "Pause", Pause);
  Nan::SetPrototypeMethod(tpl, "update_buyer_terms", UpdateBuyerTerms);
  Nan::SetPrototypeMethod(tpl, "update_seller_terms", UpdateSellerTerms);
  Nan::SetPrototypeMethod(tpl, "to_observe_mode", ToObserveMode);
  Nan::SetPrototypeMethod(tpl, "to_sell_mode", ToSellMode);
  Nan::SetPrototypeMethod(tpl, "to_buy_mode", ToBuyMode);
  Nan::SetPrototypeMethod(tpl, "post_torrent_plugin_status_updates", PostTorrentPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "post_peer_plugin_status_updates", PostPeerPluginStatusUpdates);
  Nan::SetPrototypeMethod(tpl, "stop_all_torrent_plugins", StopAllTorrentPlugins);
  Nan::SetPrototypeMethod(tpl, "pause_libtorrent", PauseLibtorrent);
  Nan::SetPrototypeMethod(tpl, "add_torrent", AddTorrent);
  Nan::SetPrototypeMethod(tpl, "remove_torrent", RemoveTorrent);
  Nan::SetPrototypeMethod(tpl, "pause_torrent", PauseTorrent);
  Nan::SetPrototypeMethod(tpl, "resume_torrent", ResumeTorrent);
  Nan::SetPrototypeMethod(tpl, "start_downloading", StartDownloading);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("Plugin").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

v8::Local<v8::Object> plugin::NewIntance(const boost::shared_ptr<extension::Plugin> & plugin) {

}

NAN_METHOD(plugin::New) {

  //auto new Plugin(uint minimumMessageId);

}

NAN_METHOD(plugin::AlertConverter) {

  //Grab PluginAlertToObjectConverter::converter from somwhere
  //return PluginAlertToObjectConverter::NewInstance(conf);
}

NAN_METHOD(plugin::Start) {

}

NAN_METHOD(plugin::Stop) {

}

NAN_METHOD(plugin::Pause) {

}

NAN_METHOD(plugin::UpdateBuyerTerms) {

}

NAN_METHOD(plugin::UpdateSellerTerms) {

}

NAN_METHOD(plugin::ToObserveMode) {

}

NAN_METHOD(plugin::ToSellMode) {

}

NAN_METHOD(plugin::ToBuyMode) {

}

NAN_METHOD(plugin::PostTorrentPluginStatusUpdates) {

}

NAN_METHOD(plugin::PostPeerPluginStatusUpdates) {

}

NAN_METHOD(plugin::StopAllTorrentPlugins) {

}

NAN_METHOD(plugin::PauseLibtorrent) {

}

NAN_METHOD(plugin::AddTorrent) {

}

NAN_METHOD(plugin::RemoveTorrent) {

}

NAN_METHOD(plugin::PauseTorrent) {

}

NAN_METHOD(plugin::ResumeTorrent) {

}

NAN_METHOD(plugin::StartDownloading) {

}

}
}
