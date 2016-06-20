/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_TORRENT_HPP
#define JOYSTREAM_CORE_DETAIL_TORRENT_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <cstdint>

namespace joystream {
namespace core {

class Node;
class Torrent;

namespace detail {

class Torrent {

public:

    enum class State {

        // Torrent has been libtorrent::async_add_torrent has been called, without response
        waiting_for_async_added,

        // Torrent was successfully added to session.
        // Can be set either from successfull libtorrent::add_torrent_alert,
        // or synchronous adding which succeeds.
        waiting_for_torrent_to_be_checked,

        // When a torrent is added, some checking is done
        // - primarily resume data I think, and when that is completed
        // the libtorrent::torrent_checked_alert alert is issued by libtorrent
        // torrent_checked,

        // <== this should probably be removed
        nothing,

        // Result of some pending resume_data() call,
        // can be either
        // libtorrent::save_resume_data_alert
        // libtorrent::save_resume_data_failed_alert
        waiting_for_resume_data
    };

    Torrent(core::Node * node,
            const libtorrent::sha1_hash &infoHash,
            const std::string & name,
            const std::string & savePath,
            const std::vector<char> & resumeData,
            std::uint64_t flags,
            State state);

    /// Plugin actions

    void start();

    void stop();

    void pause();

    void updateTerms(const protocol_wire::BuyerTerms & terms);

    void updateTerms(const protocol_wire::SellerTerms & terms);

    void toObserveMode();

    void toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                    const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                    const protocol_session::SellingPolicy & sellingPolicy,
                    const protocol_wire::SellerTerms & terms);

    void toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                   const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                   const Coin::UnspentP2PKHOutput & funding,
                   const protocol_session::BuyingPolicy & policy,
                   const protocol_wire::BuyerTerms & terms);

    /**
    // Add plugins
    void addPlugin(const SellerTorrentPlugin::Status & status);
    void addPlugin(const BuyerTorrentPlugin::Status & status);
    */

    // Getters and Setters
    libtorrent::sha1_hash infoHash() const;

    std::string name() const;
    void setName(const std::string & name);

    std::string savePath() const;
    void setSavePath(const std::string & savePath);

    std::vector<char> resumeData() const;
    void setResumeData(const std::vector<char> & resumeData);

    std::uint64_t flags() const;
    void setFlags(std::uint64_t flags);

    //configuration::Torrent configuration() const;

    libtorrent::torrent_handle handle() const;

    State state() const;
    void setState(State state);

    std::weak_ptr<core::Torrent> model();

    /**
    // Stream management
    void addStream(Stream * stream);
    void removeStream(Stream * stream);

    // Given piece was read
    void pieceRead(const boost::shared_array<char> & buffer, int pieceIndex, int size);

    // Given piece was downloaded and checked
    void pieceFinished(int piece);
    */

private:

    // Node
    core::Node * _node;

    // Info hash of torrent
    libtorrent::sha1_hash _infoHash;

    // Display name
    std::string _name;

    // Save path
    std::string _savePath;

    // Resume data
    std::vector<char> _resumeData;

    // Flags

    std::uint64_t _flags;

    /**
    // Handle to torrent
    // A valid handle is only set after the torrent has been added
    // successfully to session
    libtorrent::torrent_handle _handle;
    */

    // Status
    State _status;

    // Add const pointer to const object of type TorrentPlugin in the future?
    // can be used to look at stuff like plugin mode etc.
    // worth looking at.

    // Public interface of torrent
    // We opt for shared pointer, rather than by value,
    // so that client can be given corresponding weak_ptr.
    // Giving a raw pointer can allow client to access object
    // after it is deleted. The controller::removedTorrent signal helps avoid
    // that, but it is not perfect, as client cannot be guaranteed to see signal in time.
    std::shared_ptr<core::Torrent> _model;

    // All streams for this torrent.
    // Not quite sure if multiple separate streams for one torrent
    // is necessary, if not, then remove this QSet later.
    //QSet<Stream *> _streams;
};

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_TORRENT_HPP

