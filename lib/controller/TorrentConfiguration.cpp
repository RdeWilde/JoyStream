#include "TorrentConfiguration.hpp"
#include "Exceptions/InvalidBitSwaprStateEntryException.hpp"

TorrentConfiguration::TorrentConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags)
    : info_hash_(info_hash)
    , name_(name)
    , save_path_(save_path)
    , resume_data_(resume_data)
    , flags_(flags) {
}

TorrentConfiguration::TorrentConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry) {

    // Check that info_hash is present
    if(dictionaryEntry.count("info_hash") == 1) {

        // Get entry
        libtorrent::entry infoHashEntry = dictionaryEntry.find("info_hash")->second;

        // Check that entry is of type entry::string_type
        if(infoHashEntry.type() == libtorrent::entry::data_type::string_t)
            info_hash_ = libtorrent::sha1_hash(infoHashEntry.string()); // Why do we need to call constructor, why no conversion?
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key is not of type entry::string_type.");

    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "info_hash key should have .count == 1.");

    // Check that name is present
    if(dictionaryEntry.count("name") == 1) {

        // Get entry
        libtorrent::entry nameEntry = dictionaryEntry.find("name")->second;

        // Check that entry is of type entry::string_type
        if(nameEntry.type() == libtorrent::entry::data_type::string_t)
            name_ = nameEntry.string();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "name key should have .count == 1.");

    // Check that save_path is present
    if(dictionaryEntry.count("save_path") == 1) {

        // Get entry
        libtorrent::entry savePathEntry = dictionaryEntry.find("save_path")->second;

        // Check that entry is of type entry::string_type
        if(savePathEntry.type() == libtorrent::entry::data_type::string_t)
            save_path_ = savePathEntry.string();
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
        if(resumeDataEntry.type() == libtorrent::entry::data_type::string_t) {

            // Use as entry::string_type
            libtorrent::entry::string_type resumeDataStringEntry = resumeDataEntry.string();

            // Populate resume_data_ vector
            for(std::string::const_iterator i = resumeDataStringEntry.begin(),
                    end(resumeDataStringEntry.end());i != end;i++)
                    resume_data_.push_back(*i);

        } else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key is not of type entry::string_type.");
    }
    else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "save_path key should have .count == 1.");

    // Check that flags is present
    if(dictionaryEntry.count("flags") == 1) {

        // Get entry
        libtorrent::entry flagsEntry = dictionaryEntry.find("flags")->second;

        // Check that entry is of type entry::integer_type
        if(flagsEntry.type() == libtorrent::entry::data_type::int_t)
            flags_ = flagsEntry.integer();
        else
            throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key is not of type entry::integer_type.");
    } else
        throw InvalidBitSwaprStateEntryException(dictionaryEntry, "flags key should have .count == 1.");
}

TorrentConfiguration & TorrentConfiguration::operator=(const TorrentConfiguration & rhs) {

    // Copy fields
    info_hash_ = rhs.getInfoHash();
    name_ = rhs.getName();
    save_path_ = rhs.getSavePath();
    resume_data_ = rhs.getConstResumeData();
    flags_ = rhs.getFlags();

    return *this;
}

void TorrentConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // info_hash
    dictionaryEntry["info_hash"] = libtorrent::entry::string_type(info_hash_.to_string());

    // name
    dictionaryEntry["name"] = libtorrent::entry::string_type(name_);

    // save_path
    dictionaryEntry["save_path"] = libtorrent::entry::string_type(save_path_);

    // resume_data
    std::string resume_data_string;
    for(std::vector<char>::const_iterator i = resume_data_.begin(),
        end(resume_data_.end());i != end;i++)
            resume_data_string.append(&(*i));

    dictionaryEntry["resume_data"] = libtorrent::entry::string_type(resume_data_string);

    // flags
    dictionaryEntry["flags"] = libtorrent::entry::integer_type(flags_);
}

const libtorrent::sha1_hash & TorrentConfiguration::getInfoHash() const {
    return info_hash_;
}

const std::string & TorrentConfiguration::getName() const {
     return name_;
}

const std::string & TorrentConfiguration::getSavePath() const {
    return save_path_;
}

std::vector<char> & TorrentConfiguration::getResumeData() {
    return resume_data_;
}

const std::vector<char> & TorrentConfiguration::getConstResumeData() const {
    return resume_data_;
}

quint64 TorrentConfiguration::getFlags() const {
    return flags_;
}
