/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Lola Rigaut-Luczak <rllola80@gmail.com>, January 18 2017
 */

#include "add_torrent_params.hpp"
#include "torrent_info.h"
#include "sha1_hash.hpp"
#include "utils.hpp"

#define TI_KEY "ti"
#define NAME_KEY "name"
#define SAVE_PATH_KEY "savePath"
#define INFO_HASH_KEY "infoHash"
#define URL_KEY "url"
#define RESUME_DATA_KEY "resumeData"
#define UPLOAD_LIMIT_KEY "uploadLimit"
#define DOWNLOAD_LIMIT_KEY "downloadLimit"

#include <libtorrent/add_torrent_params.hpp>

namespace libtorrent {
namespace node {
namespace add_torrent_params {

v8::Local<v8::Object> encode(const libtorrent::add_torrent_params & atp) {

    v8::Local<v8::Object> o = Nan::New<v8::Object>();

    //SET_VAL(o, TI_KEY, TorrentInfoWrap::New(atp.ti));
    SET_STD_STRING(o, NAME_KEY, atp.name);
    SET_STD_STRING(o, SAVE_PATH_KEY, atp.save_path);
    SET_VAL(o, INFO_HASH_KEY, libtorrent::node::sha1_hash::encode(atp.info_hash));
    SET_STD_STRING(o, URL_KEY, atp.url);
    SET_STD_STRING(o, RESUME_DATA_KEY, std::string(atp.resume_data.begin(), atp.resume_data.end()));
    SET_INT32(o, UPLOAD_LIMIT_KEY, atp.upload_limit);
    SET_INT32(o, DOWNLOAD_LIMIT_KEY, atp.download_limit);

    return o;
}

libtorrent::add_torrent_params fromObject(const v8::Local<v8::Object> & o) {
  libtorrent::add_torrent_params atp;

  // atp.ti =
  atp.name =  GET_STD_STRING(o, NAME_KEY);
  atp.save_path =  GET_STD_STRING(o, SAVE_PATH_KEY);
  // atp.info_hash
  atp.url =  GET_STD_STRING(o, URL_KEY);
  std::string str = GET_STD_STRING(o, RESUME_DATA_KEY);
  std::copy(str.begin(), str.end(), std::back_inserter(atp.resume_data));
  atp.upload_limit =  GET_INT32(o, UPLOAD_LIMIT_KEY);
  atp.download_limit =  GET_INT32(o, DOWNLOAD_LIMIT_KEY);

  return atp;
}

}}}
