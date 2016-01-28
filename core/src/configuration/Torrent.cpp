/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 27 2016
 */

#include <core/configurations/Torrent.hpp>

//#include <core/exceptions/InvalidBitSwaprStateEntryException.hpp>
//#include <core/extension/PluginMode.hpp>

namespace joystream {
namespace core {
namespace configuration {

    Torrent::Torrent() {
    }

    Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                                               const std::string & name,
                                               const std::string & savePath,
                                               const std::vector<char> & resumeData,
                                               quint64 flags,
                                               //const libtorrent::torrent_info & torrentInfo
                                               const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                                               const std::string url)
                                               //const TorrentPlugin::Configuration * torrentPluginConfiguration)
        :_infoHash(infoHash)
        ,_name(name)
        ,_savePath(savePath)
        ,_resumeData(resumeData)
        ,_flags(flags)
        ,_torrentFile(torrentFile)
        ,_magnetLink(url) {
        //,_torrentPluginConfiguration(torrentPluginConfiguration) {
    }

    Torrent::~Torrent() {

        //if(_torrentFile != NULL)
            //delete _torrentFile;
    }

    Torrent Torrent::fromTorrentFile(const QString & resource) {
        // Error code
        libtorrent::error_code ec;

        // Load torrent file
        libtorrent::torrent_info * torrentInfo = new libtorrent::torrent_info(resource.toStdString().c_str(), ec);

        // Was torrent file valid?
        if(ec) {
            qDebug() << "Invalid torrent file: " << ec.message().c_str();
            throw (std::runtime_error("Invalid Torrent File"));
        }

        // Resume data
        std::vector<char> resume_data;

        // Save Path
        std::string save_path = "";

        // Null Magnet Link
        std::string url = "";

        Torrent configuration(torrentInfo->info_hash(),
                                                          torrentInfo->name(),
                                                          save_path,
                                                          resume_data,
                                                          libtorrent::add_torrent_params::flag_update_subscribe,
                                                          torrentInfo,
                                                          url);

        return configuration;
    }

    Torrent Torrent::fromMagnetLink(const QString & resource) {
        // Error code
        libtorrent::error_code ec;

        // Magnet link url
        std::string url = resource.toStdString();

        // parse_magnet_uri
        libtorrent::add_torrent_params params;

        // Parse link to get info_hash
        libtorrent::parse_magnet_uri(url, params, ec);

        // Was magnet link malformed
        if(ec) {
            qDebug() << "Invalid magnet link: " << ec.message().c_str();
            throw (std::runtime_error("Invalid Magnet Link"));
        }

        // Resume data
        std::vector<char> resume_data;

        // Save Path
        std::string save_path = "";

        libtorrent::torrent_info * torrentInfo = new libtorrent::torrent_info(params.info_hash);

        Torrent configuration(params.info_hash,
                                                          params.name,
                                                          save_path,
                                                          resume_data,
                                                          libtorrent::add_torrent_params::flag_update_subscribe,
                                                          torrentInfo,
                                                          url);

        return configuration;
    }

    /*

    TorrentConfiguration & TorrentConfiguration::operator=(const TorrentConfiguration & rhs) {

        // Copy members
        _infoHash = rhs.getInfoHash();
        _name = rhs.getName();
        _savePath = rhs.getSavePath();
        _resumeData = rhs.getConstResumeData();
        _flags = rhs.getFlags();

        // Delete if was set to object
        if(_torrentInfo != NULL)
            delete _torrentInfo;

        // Allocate copy for new object
        _torrentInfo = new libtorrent::torrent_info(*rhs.getTorrentInfo());

        _torrentPluginConfiguration = rhs.getTorrentPluginConfiguration();

        // Return self reference
        return *this;
    }
    */

    Torrent::Torrent(const libtorrent::entry::dictionary_type & dictionaryEntry) {
        //: _torrentInfo(libtorrent::sha1_hash()){

        /**
        // Check that info_hash is present
        if(dictionaryEntry.count("info_hash") == 1) {

            // Get entry
            libtorrent::entry infoHashEntry = dictionaryEntry.find("info_hash")->second;

            // Check that entry is of type entry::string_t
            if(infoHashEntry.type() == libtorrent::entry::string_t)
                _infoHash = libtorrent::sha1_hash(infoHashEntry.string()); // Why do we need to call constructor, why no conversion?
            else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key is not of type entry::string_type.");

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key should have .count == 1.");

        // Check that name is present
        if(dictionaryEntry.count("name") == 1) {

            // Get entry
            libtorrent::entry nameEntry = dictionaryEntry.find("name")->second;

            // Check that entry is of type entry::string_t
            if(nameEntry.type() == libtorrent::entry::string_t)
                _name = nameEntry.string();
            else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key is not of type entry::string_type.");
        }
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key should have .count == 1.");

        // Check that save_path is present
        if(dictionaryEntry.count("save_path") == 1) {

            // Get entry
            libtorrent::entry savePathEntry = dictionaryEntry.find("save_path")->second;

            // Check that entry is of type entry::string_t
            if(savePathEntry.type() == libtorrent::entry::string_t)
                _savePath = savePathEntry.string();
            else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
        }
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");


        // Check that resume_data is present
        if(dictionaryEntry.count("resume_data") == 1) {

            // Get entry
            libtorrent::entry resumeDataEntry = dictionaryEntry.find("resume_data")->second;

            // Check that entry is of type entry::string_type
            if(resumeDataEntry.type() == libtorrent::entry::string_t) {

                // Use as entry::string_type
                libtorrent::entry::string_type resumeDataStringEntry = resumeDataEntry.string();

                // Populate resume_data_ vector
                for(std::string::const_iterator i = resumeDataStringEntry.begin(),
                        end(resumeDataStringEntry.end());i != end;i++)
                        _resumeData.push_back(*i);

            } else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
        }
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");

        // Check that flags is present
        if(dictionaryEntry.count("flags") == 1) {

            // Get entry
            libtorrent::entry flagsEntry = dictionaryEntry.find("flags")->second;

            // Check that entry is of type entry::int_t
            if(flagsEntry.type() == libtorrent::entry::int_t)
                _flags = flagsEntry.integer();
            else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key is not of type entry::int_t.");
        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key should have .count == 1.");

        // Check if torrentInfo is present
        // NOT IMPLEMENTED

        // Check if torrentPluginConfiguration is present
        if(dictionaryEntry.count("configuration") == 1) {

            // Get entry
            libtorrent::entry configurationEntry = dictionaryEntry.find("configuration")->second;

            // Check that entry is of type entry::dictionary_t
            if(configurationEntry.type() == libtorrent::entry::dictionary_t) {

                //_configuration = TorrentPlugin::Configuration(configurationEntry.dict());

                libtorrent::entry::dictionary_type configurationDictionaryEntry = configurationEntry.dict();

                // Figure out mode of plugin
                PluginMode mode;

                try {
                    mode = TorrentPlugin::Configuration::pluginMode(configurationDictionaryEntry);
                } catch (InvalidBitSwaprStateEntryException & e) {

                }

                // Create corresponding
                switch(mode) {

                    case PluginMode::Buyer:
                        _configuration = new BuyerTorrentPlugin::Configuration(configurationDictionaryEntry);
                        break;
                    case PluginMode::Seller:
                        _configuration = new SellerTorrentPlugin::Configuration(configurationDictionaryEntry);
                        break;
                    case PluginMode::Observer:
                        _configuration = NULL; // LATER
                        break;
                }


            } else
                throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key is not of type entry::dictionary_t.");
        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key should have .count == 1.");

    */
    }

    void Torrent::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

        /**
        // _infoHash
        dictionaryEntry["infoHash"] = libtorrent::entry::string_type(_infoHash.to_string());

        // _name
        dictionaryEntry["name"] = libtorrent::entry::string_type(_name);

        // _savePath
        dictionaryEntry["savePath"] = libtorrent::entry::string_type(_savePath);

        // _resumeData
        std::string resume_data_string;
        for(std::vector<char>::const_iterator i = _resumeData.begin(),
            end(_resumeData.end());i != end;i++)
                resume_data_string.append(&(*i));

        dictionaryEntry["resumeData"] = libtorrent::entry::string_type(resume_data_string);

        // _flags
        dictionaryEntry["flags"] = libtorrent::entry::integer_type(_flags);

        // _torrentInfo
        // NOT IMPLEMENTED
        //dictionaryEntry["torrentInfo"] = ;

        // _configuration
        libtorrent::entry::dictionary_type configurationDictionaryEntry;
        _configuration->toDictionaryEntry(configurationDictionaryEntry);
        dictionaryEntry["configuration"] = configurationDictionaryEntry;
        */
    }

    libtorrent::add_torrent_params Torrent::toAddTorrentParams() const {

        // Create add_torrent_params for adding
        libtorrent::add_torrent_params params;

        params.info_hash = _infoHash;
        params.name = _name;
        params.save_path = _savePath;
        params.resume_data = _resumeData;// new std::vector<char>(_resumeData); // We do not own this pointer
        params.flags = _flags;
        params.ti = _torrentFile;
        params.url = _magnetLink;

        //if(!_torrentInfo.info_hash().is_all_zeros())
        //    params.ti = boost::intrusive_ptr<libtorrent::torrent_info>(new libtorrent::torrent_info(_torrentInfo));

        //params.userdata = static_cast<void *>(_torrentPluginConfiguration);

        // Return parameters
        return params;
    }

    boost::intrusive_ptr<libtorrent::torrent_info> Torrent::torrentFile() const {
        return _torrentFile;
    }

    /**
    const TorrentPlugin::Configuration * TorrenttorrentPluginConfiguration() const {
        return _torrentPluginConfiguration;
    }

    void TorrentsetTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration) {
        _torrentPluginConfiguration = torrentPluginConfiguration;
    }

    libtorrent::torrent_info TorrenttorrentInfo() const {
        return _torrentInfo;
    }

    void TorrentsetTorrentInfo(libtorrent::torrent_info * torrentInfo) {
        _torrentInfo = torrentInfo;
    }
    */

    quint64 Torrent::flags() const {
        return _flags;
    }

    void Torrent::setFlags(quint64 flags) {
        _flags = flags;
    }

    std::vector<char> TorrentresumeData() const {
        return _resumeData;
    }

    void Torrent::setResumeData(const std::vector<char> & resumeData) {
        _resumeData = resumeData;
    }

    std::string Torrent::savePath() const {
        return _savePath;
    }

    void Torrent::setSavePath(const std::string & savePath) {
        _savePath = savePath;
    }

    std::string Torrent::name() const {
        return _name;
    }

    void Torrent::setName(const std::string & name) {
        _name = name;
    }

    libtorrent::sha1_hash Torrent::infoHash() const {
        return _infoHash;
    }

    void Torrent::setInfoHash(const libtorrent::sha1_hash & infoHash) {
        _infoHash = infoHash;
    }

}
}
}
