#ifndef BUYQUEUE_HPP
#define BUYQUEUE_HPP

#include <libtorrent/sha1_hash.hpp>
#include <protocol_wire/BuyerTerms.hpp>
#include <protocol_session/BuyingPolicy.hpp>
#include <app_kit/AppKit.hpp>

class BuyQueue
{
    struct Item {
        joystream::protocol_wire::BuyerTerms terms;
        joystream::protocol_session::BuyingPolicy policy;
        uint tries = 0;
    };

public:
    BuyQueue(joystream::appkit::AppKit* kit);

    void add(libtorrent::sha1_hash, joystream::protocol_wire::BuyerTerms terms, joystream::protocol_session::BuyingPolicy policy);

private:
    std::map<libtorrent::sha1_hash, Item> _queue;
    joystream::appkit::AppKit *_kit;

    void buyTorrent(libtorrent::sha1_hash);
    void startTorrent(joystream::core::Torrent*);
};

#endif // BUYQUEUE_HPP
