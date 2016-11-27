#ifndef LIBTORRENT_INTERFACE_ALERT_MANAGER_HPP
#define LIBTORRENT_INTERFACE_ALERT_MANAGER_HPP

#include <boost/config.hpp>
#include <boost/cstdint.hpp>

#include <libtorrent/torrent.hpp>

#include "AlertManagerInterface.hpp"

namespace libtorrent {
    class torrent;
    class alert;
    class alert_manager;
}

namespace joystream {

namespace extension {
    namespace alert {
        struct PluginStatus;
        struct RequestResult;
        typedef std::function<void()> LoadedCallBack;
    }
    class Plugin;
}

namespace libtorrent_interface {

class AlertManager : public AlertManagerInterface {
public:
    AlertManager(
        int queue_limit
        //boost::uint32_t alert_mask = libtorrent::alert::error_notification
    );

    ~AlertManager() {}

    virtual bool pending() const;
    virtual int  numQueuedResume() const;
    virtual void getAll(std::vector<libtorrent::alert*>& alerts, int& num_resume);

    virtual libtorrent::alert_manager native_handle() const;

    /*
     * templated methods are not directly mockable by gmock.
     * A possible workaround this problem is to write a wrapper
     * method for the templated method and mock the wrapper instead.
     */
    // NOTE: shouldPost() method is not required by extension at this
    //       stage and hence not worth working around.
    //template<class T> bool shouldPost() const;

    /*
     * It is not directly possible to mock variadic methods
     * using gmock. The workaround suggested would be to
     * overload the functions with the arguments that we require
     */
    //template<class T, typename... Args> void emplace_alert(Args&&... args);

    /*
     * Overloaded methods for the variadic template
     * emplace_alert() method.
     */
    virtual void emplace_alert(extension::Plugin *pl);
    virtual void emplace_alert(extension::alert::LoadedCallBack &c);

    /*
     * TODO: Setup Coin libs and includes and
     *       implement the below overloaded call.
    void emplace_alert(
        libtorrent::torrent_handle h,
        libtorrent::tcp::endpoint &endPoint,
        quint64 value,
        const Coin::typesafeOutPoint &anchor,
        const Coin::PublicKey &contractPk,
        const Coin::PubKeyHash &finalPkHash
    );
     */

private:
    libtorrent::alert_manager _alertManager;
};

}
}

#endif // LIBTORRENT_INTERFACE_ALERT_MANAGER_HPP
