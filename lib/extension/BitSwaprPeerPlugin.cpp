
#include "BitSwaprPeerPlugin.hpp"
#include "BitSwaprTorrentPlugin.hpp"
#include "BitSwaprPlugin.hpp"
#include "controller/Controller.hpp" // needed to connect
#include "Config.hpp"

#include <iostream>

const char * message_names[] = {
    "list",
    "offer",
    "setup_begin",
    "setup_begin_accept",
    "setup_contract",
    "setup_contract_signed",
    "setup_refund",
    "setup_refund_signed",
    "setup_contract_published",
    "setup_completed",
    "piece_get",
    "piece_missing",
    "piece_put",
    "payment",
    "end"
};


BitSwaprPeerPlugin::BitSwaprPeerPlugin(BitSwaprTorrentPlugin * torrentPlugin, libtorrent::peer_connection * peerConnection, QLoggingCategory * category)
    : torrentPlugin_(torrentPlugin)
    , peerConnection_(peerConnection)
    , peerBEP10SupportedStatus(unknown)
    , peerBEP43SupportedStatus(unknown)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {


    // Setup signals
    Controller * controller = torrentPlugin_->getPlugin()->getController();

    qRegisterMetaType<libtorrent::peer_connection *>();
    QObject::connect(this,
                     SIGNAL(peerAdded(libtorrent::peer_connection *)),
                     controller,
                     SLOT(extensionPeerAdded(libtorrent::peer_connection *)));
}

BitSwaprPeerPlugin::~BitSwaprPeerPlugin() {

}

char const * BitSwaprPeerPlugin::type() const {
    return "BitSwapr";
}

/*
 * Can add entries to the extension handshake this is not called for web seeds
 */
void BitSwaprPeerPlugin::add_handshake(libtorrent::entry & handshake) {

    // We can safely assume hanshake has proper structure, that is
    // 1) is dictionary entry
    // 2) has key m which maps to a dictionary entry

    // Add key for bitswapr payment protocol
    libtorrent::entry::dictionary_type & m = handshake["m"].dict();

    // Starting point from where to map.
    // So this is a bit of a mess. First I attempted to just look at all prexisting registrations,
    // and make sure that I started after the greatest one. However, we cannot be sure that add_handshake
    // is actually called last on our extension, and since the other extensions dont give a FUCK
    // about not overwriting other peoples extensions values. The only solution is then to just start
    // on some huge value which has no other extensions above it, so this value was found by trial and error.
    int maxExistingID = 60;

    /*
    // Iterate m key dictionary and find the greatest ID
    for(std::map<std::string, libtorrent::entry>::iterator i = m.begin(),end(m.end());i != end;i++)
        maxExistingID = std::max((int)(((*i).second).integer()), maxExistingID);
    */

    // Set m dictionary key for client
    for(int i = 0;i < NUMBER_OF_MESSAGES;i++)
        m[message_names[i]] = (maxExistingID + 1) + i;

    /*
    // Diagnostics
    qCDebug(CATEGORY) << "m:";
    for(std::map<std::string, libtorrent::entry>::iterator i = m.begin(), end(m.end());i != end;i++)
        qCDebug(CATEGORY) << ((*i).first).c_str() << " : " << ((*i).second).integer();
    */

    // Add client identification
    QString clientIdentifier = QString("BitSwapr ")
                                + QString::number(BITSWAPR_VERSION_MAJOR)
                                + QString(".")
                                + QString::number(BITSWAPR_VERSION_MINOR);

    handshake["v"] = clientIdentifier.toStdString().c_str();
}

/*
 * This is called when the initial BASIC BT handshake is received.
 * Returning false means that the other end doesn't support this
 * extension and will remove it from the list of plugins.
 * this is not called for web seeds.
 *
 * The BEP10 docs say:
 * The bit selected for the extension protocol is bit 20 from
 * the right (counting starts at 0).
 * So (reserved_byte[5] & 0x10) is the expression to use for
 * checking if the client supports extended messaging.
 */
bool BitSwaprPeerPlugin::on_handshake(char const * reserved_bits) {

    // Check if BEP10 is enabled
    if(reserved_bits[5] & 0x10) {
        peerBEP10SupportedStatus = supported;
        return true;
    } else {
        peerBEP10SupportedStatus = not_supported;
        return false;
    }
}

/*
 * Called when the extension handshake from the other end is received
 * if this returns false, it means that this extension isn't supported
 * by this peer. It will result in this peer_plugin being removed from
 * the peer_connection and destructed. this is not called for web seeds
 */
bool BitSwaprPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    // Check that BEP10 was actually supported, if
    // it wasnt, then the peer is misbehaving
    if(peerBEP10SupportedStatus != supported) {
        qCWarning(CATEGORY) << "Peer didn't support BEP10, but it sent extended handshake.";
        peerBEP43SupportedStatus = not_supported;
        return false;
    }

    // We cannot trust structure of entry, since it is from peer,
    // hence we must check it properly.

    // If its not a dictionary, we are done
    if(handshake.type() != libtorrent::lazy_entry::dict_t) {
        peerBEP43SupportedStatus = not_supported;
        return false;
    }

    // Try to extract m key, if its not present, then we are done
    const libtorrent::lazy_entry * mKey = handshake.dict_find_dict("m");
    if(!mKey) {
        peerBEP43SupportedStatus = not_supported;
        return false;
    }

    // Check that "m" key maps message id of all required messages
    for(int i = 0;i < NUMBER_OF_MESSAGES;i++) {

        // Get value of message name
        int peerMessageBEP10ID = mKey->dict_find_int_value(message_names[i], -1);

        // We are done if message was not in dictionary
        if(peerMessageBEP10ID == -1) {
            peerBEP43SupportedStatus = not_supported;
            return false;
        } else
            peerMessageMapping[i] = peerMessageBEP10ID;
    }

    // Notify
    const char * peerAddress = peerConnection_->remote().address().to_string().c_str();
    short port = peerConnection_->remote().port();

    qCDebug(CATEGORY) << "Found extension handshake for peer " << peerAddress << ":" << port << ".\n";

    // All messages were present, hence the protocol is supported
    peerBEP43SupportedStatus = supported;

    // Send signal about new peer
    emit peerAdded(peerConnection_);

    // Tell libtorrent that our extension should be kept in the loop for this peer
    return true;
}

/*
 *. m_pc.disconnect(errors::pex_message_too_large, 2);
 * m_pc.disconnect(errors::too_frequent_pex);
 * m_pc.remote().address()
 *
 */

void BitSwaprPeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

}

void BitSwaprPeerPlugin::on_connected() {

}

/*
 * Returning true from any of the message handlers indicates that the
 * plugin has handeled the message. it will break the plugin chain
 * traversing and not let anyone else handle the message, including the default handler.
 */

bool BitSwaprPeerPlugin::on_have(int index) {
    return true;
}

bool BitSwaprPeerPlugin::on_bitfield(libtorrent::bitfield const & bitfield) {
    return true;
}

bool BitSwaprPeerPlugin::on_have_all() {
    return true;
}

bool BitSwaprPeerPlugin::on_reject(libtorrent::peer_request const & peerRequest) {
    return true;
}

bool BitSwaprPeerPlugin::on_request(libtorrent::peer_request const & peerRequest) {
    return true;
}

bool BitSwaprPeerPlugin::on_unchoke() {
    return true;
}

bool BitSwaprPeerPlugin::on_interested() {
    return true;
}

bool BitSwaprPeerPlugin::on_allowed_fast(int index) {
    return true;
}

bool BitSwaprPeerPlugin::on_have_none() {
    return true;
}

bool BitSwaprPeerPlugin::on_choke() {
    return true;
}

bool BitSwaprPeerPlugin::on_not_interested() {
    return true;
}

bool BitSwaprPeerPlugin::on_piece(libtorrent::peer_request const& piece, libtorrent::disk_buffer_holder & data) {
    return true;
}

bool BitSwaprPeerPlugin::on_suggest(int index) {
    return true;
}

bool BitSwaprPeerPlugin::on_cancel(libtorrent::peer_request const & peerRequest) {
    return true;
}

bool BitSwaprPeerPlugin::on_dont_have(int index) {
    return true;
}

/*
 * Called after a choke message has been sent to the peer
 */
void BitSwaprPeerPlugin::sent_unchoke() {
}

/*
 * Called when libtorrent think this peer should be disconnected.
 * If the plugin returns false, the peer will not be disconnected.
 */
bool BitSwaprPeerPlugin::can_disconnect(libtorrent::error_code const & ec) {

    // CRITICAL
    return true;
}

/*
 * called when an extended message is received. If returning true,
 * the message is not processed by any other plugin and if false
 * is returned the next plugin in the chain will receive it to
 * be able to handle it this is not called for web seeds.
 */
bool BitSwaprPeerPlugin::on_extended(int length, int msg, libtorrent::buffer::const_interval body) {

    // CRITICAL
    return false;
}

/*
 * This is not called for web seeds
 */
bool BitSwaprPeerPlugin::on_unknown_message(int length, int msg, libtorrent::buffer::const_interval body) {

    // CRITICAL
    return false;
}

/*
 * Called when a piece that this peer participated in passes the hash_check
 */
void BitSwaprPeerPlugin::on_piece_pass(int index) {

}

/*
 * Called when a piece that this peer participated in fails the hash_check
 */
void BitSwaprPeerPlugin::on_piece_failed(int index) {

}

/*
 * Called aproximately once every second
 */
void BitSwaprPeerPlugin::tick() {

}

/*
 * Called each time a request message is to be sent. If true is returned,
 * the original request message won't be sent and no other plugin will have this function called.
 */
bool BitSwaprPeerPlugin::write_request(libtorrent::peer_request const & peerRequest) {
    return false;
}
