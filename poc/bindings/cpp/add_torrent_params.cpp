/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Lola Rigaut-Luczak <rllola80@gmail.com>, January 18 2017
 */

#include "add_torrent_params.hpp"
#include "utils.hpp"

#define ADD_TORRENT_PARAMS_TI_KEY "ti"
#define ADD_TORRENT_PARAMS_NAME_KEY "name"
#define ADD_TORRENT_PARAMS_SAVE_PATH_KEY "save_path"
#define ADD_TORRENT_PARAMS_INFO_HASH_KEY "info_hash"
#define ADD_TORRENT_PARAMS_URL_KEY "url"
#define ADD_TORRENT_PARAMS_RESUME_DATA_KEY "resume_data"
#define ADD_TORRENT_PARAMS_UPLOAD_LIMIT_KEY "upload_limit"
#define ADD_TORRENT_PARAMS_DOWNLOAD_LIMIT_KEY "download_limit"


namespace joystream {
namespace node_addon {
namespace add_torrent_params {

v8::Local<v8::Object> toObject(const libtorrent::add_torrent_params & atp) {
    Nan::EscapableHandleScope scope;

    v8::Local<v8::Object> AddTorrentParamsObject = Nan::New<v8::Object>();

    //SET_OBJ(AddTorrentParamsObject, ADD_TORRENT_PARAMS_TI_KEY, TorrentInfoWrap::New(atp.ti));
    SET_STD_STRING(AddTorrentParamsObject, ADD_TORRENT_PARAMS_NAME_KEY, atp.name);
    SET_STD_STRING(AddTorrentParamsObject, ADD_TORRENT_PARAMS_SAVE_PATH_KEY, atp.save_path);
    //SET_OBJ(AddTorrentParamsObject, ADD_TORRENT_PARAMS_INFO_HASH_KEY, atp.info_hash);
    SET_STD_STRING(AddTorrentParamsObject, ADD_TORRENT_PARAMS_URL_KEY, atp.url);
    SET_STD_STRING(AddTorrentParamsObject, ADD_TORRENT_PARAMS_RESUME_DATA_KEY, std::string(atp.resume_data.begin(), atp.resume_data.end()));
    SET_INT32(AddTorrentParamsObject, ADD_TORRENT_PARAMS_UPLOAD_LIMIT_KEY, atp.upload_limit);
    SET_INT32(AddTorrentParamsObject, ADD_TORRENT_PARAMS_DOWNLOAD_LIMIT_KEY, atp.download_limit);

    return scope.Escape(AddTorrentParamsObject);
}

libtorrent::add_torrent_params fromObject(const v8::Local<v8::Object> & o) {
  libtorrent::add_torrent_params atp;

  // atp.ti =
  atp.name =  GET_STD_STRING(o, ADD_TORRENT_PARAMS_NAME_KEY);
  atp.save_path =  GET_STD_STRING(o, ADD_TORRENT_PARAMS_SAVE_PATH_KEY);
  //atp.info_hash
  atp.url =  GET_STD_STRING(o, ADD_TORRENT_PARAMS_URL_KEY);
  std::string str = GET_STD_STRING(o, ADD_TORRENT_PARAMS_RESUME_DATA_KEY);
  std::copy(str.begin(), str.end(), std::back_inserter(atp.resume_data));
  atp.upload_limit =  GET_INT32(o, ADD_TORRENT_PARAMS_UPLOAD_LIMIT_KEY);
  atp.download_limit =  GET_INT32(o, ADD_TORRENT_PARAMS_DOWNLOAD_LIMIT_KEY);

  return atp;
}

}}}
