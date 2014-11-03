
#include "controller/include/Exceptions/MissingInfoHashViewRequestException.hpp"

MissingInfoHashViewRequestException::MissingInfoHashViewRequestException(const libtorrent::add_torrent_params & params_) : params(params_) {}
