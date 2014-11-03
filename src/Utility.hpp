
#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "Utility.hpp"

#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/entry.hpp>

#include <boost/filesystem.hpp>

//void addTorrentParamsToEntry(libtorrent::entry::dictionary_type & addTorrentParamsDictionaryEntry, const libtorrent::add_torrent_params & addTorrentParams);

//void entryToAddTorrentParams(libtorrent::add_torrent_params & addTorrentParams, const libtorrent::entry::dictionary_type & addTorrentParamsDictionaryEntry);

void loadEntryFromFile(const boost::filesystem::path & file, libtorrent::entry & e);

#endif
