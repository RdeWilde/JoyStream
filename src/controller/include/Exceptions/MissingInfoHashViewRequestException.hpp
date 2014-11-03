
#ifndef MISSING_INFO_HASH_VIEW_REQUEST_EXCEPTION_H
#define MISSING_INFO_HASH_VIEW_REQUEST_EXCEPTION_H

// Headers required for add_torrent_params
#include <libtorrent/entry.hpp>
#include <libtorrent/session.hpp>

class MissingInfoHashViewRequestException : public std::exception {

private:
	libtorrent::add_torrent_params params;

public:
	MissingInfoHashViewRequestException(const libtorrent::add_torrent_params & params_);
};

#endif