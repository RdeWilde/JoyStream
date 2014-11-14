
#include "extension/BitSwaprPeerPlugin.hpp"

#include <iostream>

BitSwaprPeerPlugin::BitSwaprPeerPlugin(BitSwaprTorrentPlugin * torrentPlugin, libtorrent::peer_connection * peerConnection)
    : torrentPlugin_(torrentPlugin)
    , peerConnection_(peerConnection) {

}

/*
BitSwaprPeerPlugin::BitSwaprPeerPlugin~() {

}
*/

char const * BitSwaprPeerPlugin::type() const {
    return "BitSwapr";
}

void BitSwaprPeerPlugin::add_handshake(libtorrent::entry & handshake) {

}

void BitSwaprPeerPlugin::on_disconnect(libtorrent::error_code const & ec) {

}

void BitSwaprPeerPlugin::on_connected() {

}

/*
 * This is called when the initial BT handshake is received.
 * Returning false means that the other end doesn't support this
 * extension and will remove it from the list of plugins.
 * this is not called for web seeds.
 */
bool BitSwaprPeerPlugin::on_handshake(char const * reserved_bits) {

    // Return true iff bep10 is enabled?
    return true;
}

/*
 * Called when the extension handshake from the other end is received
 * if this returns false, it means that this extension isn't supported
 * by this peer. It will result in this peer_plugin being removed from
 * the peer_connection and destructed. this is not called for web seeds
 */
bool BitSwaprPeerPlugin::on_extension_handshake(libtorrent::lazy_entry const & handshake) {

    // Return true if BitSwapr is supported
    return false;
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
