/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 7 2016
 */

#include <extension/detail.hpp>
#include <extension/Alert.hpp>
#include <extension/Exception.hpp>
#include <extension/Plugin.hpp>

namespace joystream {
namespace extension {
namespace detail {

void RequestVariantVisitor::operator()(const request::Start & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->start();
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::Stop & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->stop();
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::Pause & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->pause();
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::UpdateBuyerTerms & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [r](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->updateTerms(r.terms);
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::UpdateSellerTerms & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [r](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->updateTerms(r.terms);
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::ToObserveMode & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [r](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->toObserveMode();
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::ToSellMode & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [r](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->toSellMode(r.generateKeyPairsCallbackHandler,
                           r.generateP2PKHAddressesCallbackHandler,
                           r.sellingPolicy,
                           r.terms);
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::ToBuyMode & r) {

    auto e = runTorrentPluginRequest(r.infoHash, [r](const boost::shared_ptr<TorrentPlugin> & plugin) {
        plugin->toBuyMode(r.generateKeyPairsCallbackHandler,
                          r.generateP2PKHAddressesCallbackHandler,
                          r.funding,
                          r.policy,
                          r.terms);
    });

    sendRequestResult(std::bind(r.handler, e));
}

void RequestVariantVisitor::operator()(const request::UpdateStatus &) {
    _plugin->_session->alerts().emplace_alert<alert::PluginStatus>(_plugin->status());
}

void RequestVariantVisitor::operator()(const request::StopAllTorrentPlugins & r) {

    // Stop all torrent plugins which can be stopped
    auto pluginMap = _plugin->_plugins;

    for(auto m : pluginMap) {

        boost::shared_ptr<TorrentPlugin> plugin = m.second.lock();

        // Stop plugin if it exists and is not already stopped
        if(plugin && plugin->sessionState() != protocol_session::SessionState::stopped)
            plugin->stop();
    }

    // Send the result that we are done
    sendRequestResult(r.handler);
}

std::exception_ptr RequestVariantVisitor::runTorrentPluginRequest(const libtorrent::sha1_hash & infoHash,
                                                                  const std::function<void(const boost::shared_ptr<TorrentPlugin> &)> & f) const {

    // Make sure there is a torrent plugin for this torrent
    auto it = _plugin->_plugins.find(infoHash);

    std::exception_ptr e;

    // If there is no torrent plugin, then tell client
    if(it == _plugin->_plugins.cend())
        e = std::make_exception_ptr(exception::MissingTorrent());
    else {

        // Make sure the plugin is still valid
        boost::shared_ptr<TorrentPlugin> plugin = it->second.lock();

        assert(plugin);

        try {
            f(plugin);
        } catch (...) {
            e = std::current_exception();
        }
    }

    return e;
}

void RequestVariantVisitor::sendRequestResult(const alert::LoadedCallback & c) {
    _plugin->_session->alerts().emplace_alert<alert::RequestResult>(c);
}

}
}
}
