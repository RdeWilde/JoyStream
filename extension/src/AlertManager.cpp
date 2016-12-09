#include "extension/AlertManager.hpp"

namespace joystream {
namespace extension {

AlertManager::AlertManager(int queue_limit)
{
    libtorrent::alert_manager *altMng = new libtorrent::alert_manager(queue_limit);
    _alertManager = altMng;
}

libtorrent::alert_manager* AlertManager::native_handle() const
{
    return _alertManager;
}

void AlertManager::plugin_emplace_alert(extension::status::Plugin status)
{
    _alertManager->emplace_alert<extension::alert::PluginStatus>(status);
}

void AlertManager::request_emplace_alert(extension::alert::LoadedCallBack &c)
{
    _alertManager->emplace_alert<extension::alert::RequestResult>(c);
}

void AlertManager::anchorAnnounced_emplace_alert(
    libtorrent::torrent_handle h, libtorrent::tcp::endpoint &endPoint,
    quint64 value, const Coin::typesafeOutPoint &anchor,
    const Coin::PublicKey &contractPk, const Coin::PubKeyHash &finalPkHash
)
{
    _alertManager->emplace_alert<extension::alert::AnchorAnnounced>(h, endPoint, value, anchor, contractPk, finalPkHash);
}

/*
 * NOTE: gmock doesn't allow mocking templated methods.
 * Hence, we're dropping templated methods and
 * implementing explicit overloaded calls instead.
 *

template<class T, typename... Args>
void AlertManager::emplace_alert(Args&&... args)
{
    _alertManager.emplace_alert(args);
}

 */

}
}
