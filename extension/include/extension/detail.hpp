/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_DETAIL_HPP
#define JOYSTREAM_EXTENSION_DETAIL_HPP

#include <extension/Alert.hpp>
#include <extension/Request.hpp>
#include <boost/variant.hpp>
#include <exception>
#include <functional>

namespace joystream {
namespace extension {

class Plugin;

namespace detail {

// Variant used to allow a single request queue
typedef boost::variant<request::Start,
                       request::Stop,
                       request::Pause,
                       request::UpdateBuyerTerms,
                       request::UpdateSellerTerms,
                       request::ToObserveMode,
                       request::ToSellMode,
                       request::ToBuyMode,
                       request::UpdateStatus,
                       request::StopAllTorrentPlugins,
                       request::PauseLibtorrent,
                       request::AddTorrent> RequestVariant;

class RequestVariantVisitor : public boost::static_visitor<> {

public:

    RequestVariantVisitor(Plugin * plugin)
        : _plugin(plugin) {
    }

    void operator()(const request::Start & r);
    void operator()(const request::Stop & r);
    void operator()(const request::Pause & r);
    void operator()(const request::UpdateBuyerTerms & r);
    void operator()(const request::UpdateSellerTerms & r);
    void operator()(const request::ToObserveMode & r);
    void operator()(const request::ToSellMode & r);
    void operator()(const request::ToBuyMode & r);
    void operator()(const request::UpdateStatus & r);
    void operator()(const request::StopAllTorrentPlugins & r);
    void operator()(const request::PauseLibtorrent & r);
    void operator()(const request::AddTorrent & r);

private:

    //
    void sendRequestResult(const alert::LoadedCallback &);

    //
    std::exception_ptr runTorrentPluginRequest(const libtorrent::sha1_hash &,
                                               const std::function<void(const boost::shared_ptr<TorrentPlugin> &)> & f) const;

    Plugin * _plugin;
};

}
}
}


#endif // JOYSTREAM_EXTENSION_DETAIL_HPP
