#include "AlertManager.hpp"

namespace joystream {
namespace libtorrent_interface {

AlertManager::AlertManager(int queue_limit /*, boost::uint32_t alert_mask = libtorrent::alert::error_notification */ )
{
    //libtorrent::alert_manager altMng(queue_limit);
    libtorrent::alert_manager *altMng = new libtorrent::alert_manager(queue_limit);
    _alertManager = altMng;
}

bool AlertManager::pending() const
{
    return _alertManager->pending();
}

int AlertManager::numQueuedResume() const
{
    return _alertManager->num_queued_resume();
}

void AlertManager::getAll(std::vector<libtorrent::alert*>& alerts, int& num_resume)
{
    _alertManager->get_all(alerts, num_resume);
}

libtorrent::alert_manager* AlertManager::native_handle() const
{
    return _alertManager;
}

void AlertManager::emplace_alert(extension::Plugin *pl)
{
    _alertManager->emplace_alert<extension::alert::PluginStatus>(pl->status());
}

void AlertManager::emplace_alert(extension::alert::LoadedCallBack &c)
{
    _alertManager->emplace_alert<extension::alert::RequestResult>(c);
}

/*
 * NOTE: gmock doesn't allow mocking templated methods.
 * Hence, we're dropping templated methods and
 * implementing explicit overloaded calls instead.
 *
template<class T>
bool AlertManager::shouldPost() const
{
    return _alertManager.should_post();
}

template<class T, typename... Args>
void AlertManager::emplace_alert(Args&&... args)
{
    _alertManager.emplace_alert(args);
}
 */

}
}
