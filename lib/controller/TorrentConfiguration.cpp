#include "TorrentConfiguration.hpp"
#include "Exceptions/InvalidBitSwaprStateEntryException.hpp"

TorrentConfiguration::TorrentConfiguration(const libtorrent::sha1_hash & infoHash
                                           ,const std::string & name
                                           ,const std::string & savePath
                                           ,const std::vector<char> & resumeData
                                           ,quint64 flags
                                           ,libtorrent::torrent_info * torrentInfo
                                           ,const TorrentPlugin::Configuration & configuration)
                    :_infoHash(infoHash)
                    ,_name(name)
                    ,_savePath(savePath)
                    ,_resumeData(resumeData)
                    ,_flags(flags)
                    ,_torrentInfo(torrentInfo)
                    ,_configuration(configuration) {
}

TorrentConfiguration::~TorrentConfiguration() {

    // Delete if was set to object
    if(_torrentInfo != NULL)
        delete _torrentInfo;
}

/*
TorrentConfiguration::TorrentConfiguration()
                    : _torrentInfo(NULL) {
}

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

TorrentConfiguration::TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

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
        if(configurationEntry.type() == libtorrent::entry::dictionary_t)
            _configuration = TorrentPlugin::Configuration(configurationEntry.dict());
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key is not of type entry::dictionary_t.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "configuration key should have .count == 1.");
}

void TorrentConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

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
    _configuration.toDictionaryEntry(configurationDictionaryEntry);
    dictionaryEntry["configuration"] = configurationDictionaryEntry;
}

libtorrent::add_torrent_params TorrentConfiguration::toAddTorrentParams() const {

    // Create add_torrent_params for adding
    libtorrent::add_torrent_params params;

    params.info_hash = _infoHash;
    params.name = _name;
    params.save_path = _savePath;
    params.resume_data = new std::vector<char>(_resumeData); // We do not own this pointer
    params.flags = _flags;

    if(_torrentInfo != NULL)
        params.ti = boost::intrusive_ptr<libtorrent::torrent_info>(_torrentInfo);

    //params.userdata = static_cast<void *>(_torrentPluginConfiguration);

    // Return parameters
    return params;
}

const libtorrent::sha1_hash & TorrentConfiguration::getInfoHash() const {
    return _infoHash;
}

const std::string & TorrentConfiguration::getName() const {
     return _name;
}

const std::string & TorrentConfiguration::getSavePath() const {
    return _savePath;
}

const std::vector<char> & TorrentConfiguration::getResumeData() const {
    return _resumeData;
}

quint64 TorrentConfiguration::getFlags() const {
    return _flags;
}

const libtorrent::torrent_info * TorrentConfiguration::getTorrentInfo() const {
    return _torrentInfo;
}

/*
const TorrentPluginConfiguration * TorrentConfiguration::getTorrentPluginConfiguration() const {
    return _torrentPluginConfiguration;
}

void TorrentConfiguration::setTorrentPluginConfiguration(const TorrentPluginConfiguration & torrentPluginConfiguration) {
    _torrentPluginConfiguration = torrentPluginConfiguration;
}
*/
