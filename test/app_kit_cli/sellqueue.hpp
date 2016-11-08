#ifndef SELLQUEUE_HPP
#define SELLQUEUE_HPP

#include <libtorrent/sha1_hash.hpp>
#include <protocol_wire/SellerTerms.hpp>
#include <protocol_session/SellingPolicy.hpp>
#include <app_kit/AppKit.hpp>

class SellQueue
{
    struct Item {
        joystream::protocol_wire::SellerTerms terms;
        joystream::protocol_session::SellingPolicy policy;
        uint tries = 0;
    };

public:
    SellQueue(joystream::appkit::AppKit* kit);

    void add(libtorrent::sha1_hash, joystream::protocol_wire::SellerTerms terms, joystream::protocol_session::SellingPolicy policy);

private:
    std::map<libtorrent::sha1_hash, Item> _queue;
    joystream::appkit::AppKit *_kit;

    void sellTorrent(libtorrent::sha1_hash);
    void startTorrent(joystream::core::Torrent*);
};

#endif // SELLQUEUE_HPP
