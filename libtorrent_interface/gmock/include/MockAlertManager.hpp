#ifndef GMOCK_MOCK_ALERT_MANAGER_HPP
#define GMOCK_MOCK_ALERT_MANAGER_HPP

#include <gmock/gmock.h>

#include <extension/alert.hpp>
#include <libtorrent/socket.hpp>

#include "AlertManagerInterface.hpp"

namespace joystream {
namespace gmock {

class MockAlertManager : public libtorrent_interface::AlertManagerInterface {
public:
    MOCK_CONST_METHOD0(pending, bool());
    MOCK_CONST_METHOD0(numQueuedResume, int());
    MOCK_CONST_METHOD2(getAll, void(std::vector<libtorrent::alert*>&, int&));
    MOCK_CONST_METHOD0(native_handle, libtorrent::alert_manager());

    /*
     * Template methods can't be mocked directly in gmock.
     * We hence mock overloaded methods as per our use.
     */
    //MOCK_METHOD0(_shouldPost, bool());
    MOCK_METHOD1(emplace_alert, void(extension::status::Plugin* ));
    //MOCK_METHOD1(emplace_alert, void(extension::alert::LoadedCallBack& ));
};

}
}

#endif // GMOCK_MOCK_ALERT_MANAGER_HPP
