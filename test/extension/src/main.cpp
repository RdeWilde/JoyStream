#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "PollableInterface.hpp"
#include "Buyer.hpp"
#include "Seller.hpp"
#include <protocol_wire/protocol_wire.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <boost/asio/impl/src.hpp>
#include <functional>
#include <memory>

using namespace joystream;
/**
libtorrent::settings_pack generate_settings_pack(bool enableDHT) noexcept {

    #define CORE_PEER_ID "xy"
    #define CORE_USER_AGENT_NAME "xy name"
    #define CORE_VERSION_MAJOR 1
    #define CORE_VERSION_MINOR 2

    // Initialize with default values
    libtorrent::settings_pack pack;

    // Setup alert filtering
    int ignoredAlerts =
                        // Enables alerts on events in the DHT node. For incoming searches or bootstrapping being done etc.
                        libtorrent::alert::dht_notification +

                        // Enables alerts for when blocks are requested and completed. Also when pieces are completed.
                        libtorrent::alert::progress_notification +

                        // Enables stats_alert approximately once every second, for every active torrent.
                        // These alerts contain all statistics counters for the interval since the lasts stats alert.
                        libtorrent::alert::stats_notification +

                        // Enables debug logging alerts. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING).
                        // The alerts being posted are log_alert and are session wide.
                        libtorrent::alert::session_log_notification +

                        // Enables debug logging alerts for torrents. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING). The alerts being posted are
                        // torrent_log_alert and are torrent wide debug events.
                        libtorrent::alert::torrent_log_notification +

                        // Enables debug logging alerts for peers. These are available unless libtorrent was
                        // built with logging disabled (TORRENT_DISABLE_LOGGING). The alerts being posted are
                        // peer_log_alert and low-level peer events and messages.
                        libtorrent::alert::peer_log_notification +

                        // Enables dht_log_alert, debug logging for the DHT
                        libtorrent::alert::dht_log_notification +

                        // Enables verbose logging from the piece picker
                        libtorrent::alert::picker_log_notification;

    pack.set_int(libtorrent::settings_pack::alert_mask, libtorrent::alert::all_categories & ~ ignoredAlerts);

    // Enable all default extensions, and possibly DHT.
    pack.set_bool(libtorrent::settings_pack::enable_upnp, true);
    pack.set_bool(libtorrent::settings_pack::enable_natpmp, true);
    pack.set_bool(libtorrent::settings_pack::enable_lsd, true);
    pack.set_bool(libtorrent::settings_pack::enable_dht, enableDHT);

    // This is the client identification to the tracker.
    // The recommended format of this string is: "ClientName/ClientVersion libtorrent/libtorrentVersion".
    // This name will not only be used when making HTTP requests, but also when sending BEP10 extended handshake
    // if handshake_client_version is left blank.
    // default: "libtorrent/" LIBTORRENT_VERSION
    pack.set_str(libtorrent::settings_pack::user_agent, std::string(CORE_USER_AGENT_NAME) +
                                                        std::string("/") +
                                                        std::to_string(CORE_VERSION_MAJOR) +
                                                        std::string(".") +
                                                        std::to_string(CORE_VERSION_MINOR));

    // Client name and version identifier sent to peers in the BEP10 handshake message.
    // If this is an empty string, the user_agent is used instead.
    // default: <user_agent>
    //pack.set_str(libtorrent::settings_pack::handshake_client_version, std::string(CORE_USER_AGENT_NAME) + CORE_VERSION_MAJOR + "." + CORE_VERSION_MINOR);

    // Fingerprint for the client.
    // It will be used as the prefix to the peer_id.
    // If this is 20 bytes (or longer) it will be used as the peer-id
    // There are two encoding styles, we use Azureus style, which is most popular:
    // '-', two characters for client id, four ascii digits for version number, '-', followed by random numbers.
    // For example: '-AZ2060-'...
    // default: "-LT1100-"
    std::string peerIdString = libtorrent::fingerprint(CORE_PEER_ID, CORE_VERSION_MAJOR, CORE_VERSION_MINOR, 0, 0).to_string();

    pack.set_str(libtorrent::settings_pack::peer_fingerprint, peerIdString);

    // Determines if connections from the same IP address as existing
    // connections should be rejected or not. Multiple connections from
    // the same IP address is not allowed by default, to prevent abusive behavior by peers.
    // It may be useful to allow such connections in cases where simulations
    // are run on the same machie, and all peers in a swarm has the same IP address.
    pack.set_bool(libtorrent::settings_pack::allow_multiple_connections_per_ip, true);


    return pack;
}
*/

#define POLLING_COUNT 3
#define POLLING_SLEEP_DURATION 1*std::chrono_literals::s

TEST(IntegrationTesting, OneToOne) {

    // Setup subjects for event loop
    std::vector<PollableInterface *> subjects;

    // Create, start and add buyer
    libtorrent::session buyer_session;
    boost::shared_ptr<extension::Plugin> plugin = boost::make_shared<extension::Plugin>(1000,
                                                                                        buyer_session.native_handle(),
/**                                                                                        &buyer_session.native_handle()->alerts());
    Buyer buyer;
    libtorrent::add_torrent_params buyer_params; // set info_hash
    protocol_wire::BuyerTerms terms(1, 1, 1, 1);
    buyer.start(&buyer_session,
                plugin.get(),
                buyer_params,
                terms);


    subjects.push_back(&buyer);
*/

    /**
    /// Create, start and add seller


    // create composite class: session + managing state machine
    seller_session_management::StateMachine seller_sm(params);
    protocol_wire::SellerTerms seller_terms(1,1,1,1,1);
    libtorrent::session seller_session;
    seller_sm.apply(seller_session_management::Start("seller", terms, &seller_session));
    auto * s = boost::get<seller_session_management::Started>(seller_sm.activeState());
    subjects.push_back(s);

    // Run event loop
    RunPollerLoop(subjects, POLLING_COUNT, POLLING_SLEEP_DURATION);
*/
    // *** assert something about final states ***
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
