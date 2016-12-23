#include "torrent_handle.h"

using namespace v8;

Nan::Persistent<Function> TorrentHandleWrap::constructor;

NAN_MODULE_INIT(TorrentHandleWrap::Init) {
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(NewInstance);
  tpl->SetClassName(Nan::New("TorrentHandleWrap").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  //Nan::SetPrototypeMethod(tpl, "getPeerInfo", get_peer_info);
  //Nan::SetPrototypeMethod(tpl, "status", status);
  Nan::SetPrototypeMethod(tpl, "getDownloadQueue", get_download_queue);
  //Nan::SetPrototypeMethod(tpl, "fileProgress", file_progress);
  //Nan::SetPrototypeMethod(tpl, "trackers", trackers);
  //Nan::SetPrototypeMethod(tpl, "replaceTrackers", replace_trackers);
  //Nan::SetPrototypeMethod(tpl, "addTracker", add_tracker);
  Nan::SetPrototypeMethod(tpl, "addUrlSeed", add_url_seed);
  Nan::SetPrototypeMethod(tpl, "removeUrlSeed", remove_url_seed);
  Nan::SetPrototypeMethod(tpl, "urlSeeds", url_seeds);
  Nan::SetPrototypeMethod(tpl, "addHttpSeed", add_http_seed);
   Nan::SetPrototypeMethod(tpl, "removeHttpSeed", remove_http_seed);
   Nan::SetPrototypeMethod(tpl, "httpSeeds", http_seeds);
   //Nan::SetPrototypeMethod(tpl, "get_torrent_info", get_torrent_info);
   Nan::SetPrototypeMethod(tpl, "setMetadata", set_metadata);
   Nan::SetPrototypeMethod(tpl, "isValid", is_valid);
   //Nan::SetPrototypeMethod(tpl, "has_metadata", has_metadata);
   Nan::SetPrototypeMethod(tpl, "pause", pause);
   Nan::SetPrototypeMethod(tpl, "resume", resume);
   Nan::SetPrototypeMethod(tpl, "clearError", clear_error);
   Nan::SetPrototypeMethod(tpl, "setPriority", set_priority);
   Nan::SetPrototypeMethod(tpl, "superSeeding", super_seeding);

   Nan::SetPrototypeMethod(tpl, "autoManaged", auto_managed);
   Nan::SetPrototypeMethod(tpl, "queuePosition", queue_position);
   Nan::SetPrototypeMethod(tpl, "queuePositionUp", queue_position_up);
   Nan::SetPrototypeMethod(tpl, "queuePositionDown", queue_position_down);
   Nan::SetPrototypeMethod(tpl, "queuePositionTop", queue_position_top);
   Nan::SetPrototypeMethod(tpl, "queuePositionBottom", queue_position_bottom);

/*#ifndef TORRENT_DISABLE_RESOLVE_COUNTRIES
   Nan::SetPrototypeMethod(tpl, "resolve_countries", resolve_countries);
#endif*/

   Nan::SetPrototypeMethod(tpl, "addPiece", add_piece);
   Nan::SetPrototypeMethod(tpl, "readPiece", read_piece);
   Nan::SetPrototypeMethod(tpl, "havePiece", have_piece);
   Nan::SetPrototypeMethod(tpl, "setPieceDeadline", set_piece_deadline);
   Nan::SetPrototypeMethod(tpl, "resetPieceDeadline", reset_piece_deadline);
   Nan::SetPrototypeMethod(tpl, "pieceAvailability", piece_availability);
   Nan::SetPrototypeMethod(tpl, "piecePriority", piece_priority);
   Nan::SetPrototypeMethod(tpl, "prioritizePieces", prioritize_pieces);
   Nan::SetPrototypeMethod(tpl, "piecePriorities", piece_priorities);
   Nan::SetPrototypeMethod(tpl, "prioritizeFiles", prioritize_files);
   Nan::SetPrototypeMethod(tpl, "filePriorities", file_priorities);
   Nan::SetPrototypeMethod(tpl, "filePriority", file_priority);
   Nan::SetPrototypeMethod(tpl, "useInterface", use_interface);
   Nan::SetPrototypeMethod(tpl, "saveResume_data", save_resume_data);
   Nan::SetPrototypeMethod(tpl, "needSaveResumeData", need_save_resume_data);
   Nan::SetPrototypeMethod(tpl, "forceReannounce", force_reannounce);

#ifndef TORRENT_DISABLE_DHT
   Nan::SetPrototypeMethod(tpl, "forceDhtAnnounce", force_dht_announce);
#endif

   Nan::SetPrototypeMethod(tpl, "scrapeTracker", scrape_tracker);
   //Nan::SetPrototypeMethod(tpl, "name", name);
   Nan::SetPrototypeMethod(tpl, "setUploadMode", set_upload_mode);
   Nan::SetPrototypeMethod(tpl, "setShareMode", set_share_mode);
   Nan::SetPrototypeMethod(tpl, "flushCache", flush_cache);
   Nan::SetPrototypeMethod(tpl, "applyIpFilter", apply_ip_filter);
   Nan::SetPrototypeMethod(tpl, "setUploadLimit", set_upload_limit);
   Nan::SetPrototypeMethod(tpl, "setDownloadLimit", set_download_limit);
   Nan::SetPrototypeMethod(tpl, "uploadLimit", upload_limit);
   Nan::SetPrototypeMethod(tpl, "downloadLimit", download_limit);
   Nan::SetPrototypeMethod(tpl, "setSequentialDownload", set_sequential_download);
   Nan::SetPrototypeMethod(tpl, "connectPeer", connect_peer);
   //Nan::SetPrototypeMethod(tpl, "save_path", save_path);
   //Nan::SetPrototypeMethod(tpl, "make_magnet_link", make_magnet_link);
   Nan::SetPrototypeMethod(tpl, "setMaxUploads", set_max_uploads);
   Nan::SetPrototypeMethod(tpl, "maxUploads", max_uploads);
   Nan::SetPrototypeMethod(tpl, "setMaxConnections", set_max_connections);
   Nan::SetPrototypeMethod(tpl, "maxConnections", max_connections);
   //Nan::SetPrototypeMethod(tpl, "set_tracker_login", set_tracker_login);
   Nan::SetPrototypeMethod(tpl, "moveStorage", move_storage);
   Nan::SetPrototypeMethod(tpl, "infoHash", info_hash);
   Nan::SetPrototypeMethod(tpl, "forceRecheck", force_recheck);
   Nan::SetPrototypeMethod(tpl, "renameFile", rename_file);
   Nan::SetPrototypeMethod(tpl, "setSSLCertificate", set_ssl_certificate);

   Nan::SetPrototypeMethod(tpl, "torrentFile", torrent_file);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("TorrentHandleWrap").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
};

TorrentHandleWrap::TorrentHandleWrap() {
   th_ = NULL;
};

TorrentHandleWrap::~TorrentHandleWrap() {
  if (th_ != NULL)
    delete th_;
};


Local<Object> TorrentHandleWrap::New(const libtorrent::torrent_handle& th) {
    Nan::EscapableHandleScope scope;

    Local<Function> cons = Nan::New(constructor);
    Nan::MaybeLocal<Object> obj = cons->NewInstance(Nan::GetCurrentContext());

    Nan::ObjectWrap::Unwrap<TorrentHandleWrap>(obj.ToLocalChecked())->th_ = new libtorrent::torrent_handle(th);

    return scope.Escape(obj.ToLocalChecked());
};

NAN_METHOD(TorrentHandleWrap::NewInstance) {
  Nan::HandleScope scope;

  if (!info.IsConstructCall()) {
    Nan::ThrowTypeError("Use the new operator to create instances of this object.");
    return;
  }

  TorrentHandleWrap* obj = new TorrentHandleWrap();
  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
};

/* Don't have peer_info
NAN_METHOD(TorrentHandleWrap::get_peer_info) {
    Nan::HandleScope scope;

    std::vector<libtorrent::peer_info> res;
    TorrentHandleWrap::Unwrap(info.This())->get_peer_info(res);

    Local<Array> ret = Nan::New<Array>();

    for (std::vector<libtorrent::peer_info>::iterator i(res.begin()), e(res.end()); i != e; ++i)
        ret->Set(ret->Length(), peer_info_to_object(*i));


    info.GetReturnValue().Set(info.This());
};*/

/*
NAN_METHOD(TorrentHandleWrap::status) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());
    libtorrent::torrent_status st;

    if (info.Length() == 1)
        st = th->status(info[0]->IntegerValue());
    else
        st = th->status();

    info.GetReturnValue().Set(torrent_status_to_object(st));
};*/


NAN_METHOD(TorrentHandleWrap::get_download_queue) {
    Nan::HandleScope scope;

    std::vector<libtorrent::partial_piece_info> res;
    TorrentHandleWrap::Unwrap(info.This())->get_download_queue(res);

    Local<Array> ret = Nan::New<Array>();

    for (std::vector<libtorrent::partial_piece_info>::iterator i(res.begin()), e(res.end()); i != e; ++i) {
        Local<Object> obj = Nan::New<Object>();

        obj->Set(Nan::New("piece_index").ToLocalChecked(), Nan::New<Integer>(i->piece_index));
        obj->Set(Nan::New("blocks_in_piece").ToLocalChecked(), Nan::New<Integer>(i->blocks_in_piece));
        obj->Set(Nan::New("finished").ToLocalChecked(), Nan::New<Integer>(i->piece_index));
        obj->Set(Nan::New("writing").ToLocalChecked(), Nan::New<Integer>(i->piece_index));
        obj->Set(Nan::New("requested").ToLocalChecked(), Nan::New<Integer>(i->piece_index));

        Local<Array> blocks = Nan::New<Array>();

        for (int k = 0; k < i->blocks_in_piece; ++k) {
            Local<Object> block = Nan::New<Object>();
            Local<Array> peer = Nan::New<Array>();

            block->Set(Nan::New("state").ToLocalChecked(), Nan::New<Uint32>(i->blocks[k].state));
            block->Set(Nan::New("num_peers").ToLocalChecked(), Nan::New<Uint32>(i->blocks[k].num_peers));
            block->Set(Nan::New("bytes_progress").ToLocalChecked(), Nan::New<Uint32>(i->blocks[k].bytes_progress));
            block->Set(Nan::New("block_size").ToLocalChecked(), Nan::New<Uint32>(i->blocks[k].block_size));

            peer->Set(0, Nan::New<String>(i->blocks[k].peer().address().to_string()).ToLocalChecked());
            peer->Set(1, Nan::New<Integer>(i->blocks[k].peer().port()));

            block->Set(Nan::New("peer").ToLocalChecked(), peer);
            blocks->Set(blocks->Length(), block);
        }

        obj->Set(Nan::New("blocks").ToLocalChecked(), blocks);
        ret->Set(ret->Length(), obj);
    }

    info.GetReturnValue().Set(ret);
};

/*NAN_METHOD(TorrentHandleWrap::file_progress) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());
    std::vector<libtorrent::size_type> res;

    res.reserve(th->get_torrent_info().num_files());

    Local<Array> ret = Nan::New<Array>();

    if (info.Length() == 1)
        th->file_progress(res, info[0]->IntegerValue());
    else
        th->file_progress(res);

    for (std::vector<libtorrent::size_type>::iterator i(res.begin()), e(res.end()); i != e; ++i)
        ret->Set(ret->Length(), Nan::New<Number>(*i));

    info.GetReturnValue().Set(ret);
};*/

/*NAN_METHOD(TorrentHandleWrap::trackers) {
    Nan::HandleScope scope;

    std::vector<libtorrent::announce_entry> const res = TorrentHandleWrap::Unwrap(info.This())->trackers();

    Local<Array> ret = Nan::New<Array>();

    for (std::vector<libtorrent::announce_entry>::const_iterator i(res.begin()), e(res.end()); i != e; ++i)
        ret->Set(ret->Length(), announce_entry_to_object(*i));

    info.GetReturnValue().Set(ret);
};*/

/*NAN_METHOD(TorrentHandleWrap::replace_trackers) {
    Nan::HandleScope scope;

    std::vector<libtorrent::announce_entry> trackers;

    Local<Array> src = info[0].As<Array>();

    for (uint32_t i = 0, e = src->Length(); i < e; ++i)
        trackers.push_back(announce_entry_from_object(src->Get(i)->ToObject()));

    TorrentHandleWrap::Unwrap(info.This())->replace_trackers(trackers);

    info.GetReturnValue().SetUndefined();
};*/

/*NAN_METHOD(TorrentHandleWrap::add_tracker) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->add_tracker(announce_entry_from_object(info[0]->ToObject()));

    info.GetReturnValue().SetUndefined();
};*/

NAN_METHOD(TorrentHandleWrap::add_url_seed) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->add_url_seed(std::string(*Nan::Utf8String(info[0])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::remove_url_seed) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->remove_url_seed(std::string(*Nan::Utf8String(info[0])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::url_seeds) {
    Nan::HandleScope scope;

    std::set<std::string> urls = TorrentHandleWrap::Unwrap(info.This())->url_seeds();

    Local<Array> ret = Nan::New<Array>();

    for (std::set<std::string>::iterator i(urls.begin()), e(urls.end()); i != e; ++i)
        ret->Set(ret->Length(), Nan::New<String>(*i).ToLocalChecked());

    info.GetReturnValue().Set(ret);
};

NAN_METHOD(TorrentHandleWrap::add_http_seed) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->add_http_seed(std::string(*Nan::Utf8String(info[0])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::remove_http_seed) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->remove_http_seed(std::string(*Nan::Utf8String(info[0])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::http_seeds) {
    Nan::HandleScope scope;

    std::set<std::string> urls = TorrentHandleWrap::Unwrap(info.This())->http_seeds();

    Local<Array> ret = Nan::New<Array>();

    for (std::set<std::string>::iterator i(urls.begin()), e(urls.end()); i != e; ++i)
        ret->Set(ret->Length(), Nan::New<String>((*i).c_str()).ToLocalChecked());

    info.GetReturnValue().SetUndefined();
};

/*NAN_METHOD(TorrentHandleWrap::get_torrent_info) {
    Nan::HandleScope scope;

    libtorrent::torrent_info ti = TorrentHandleWrap::Unwrap(info.This())->get_torrent_info();

    info.GetReturnValue().Set(TorrentInfoWrap::New(ti));
};*/

NAN_METHOD(TorrentHandleWrap::set_metadata) {
    Nan::HandleScope scope;

    std::string md(*Nan::Utf8String(info[0]));

    TorrentHandleWrap::Unwrap(info.This())->set_metadata(md.c_str(), md.size());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::is_valid) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Boolean>(TorrentHandleWrap::Unwrap(info.This())->is_valid()));
};

/*NAN_METHOD(TorrentHandleWrap::has_metadata) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Boolean>(TorrentHandleWrap::Unwrap(info.This())->has_metadata()));
};*/

NAN_METHOD(TorrentHandleWrap::pause) {
    Nan::HandleScope scope;

    if (info.Length() == 1)
        TorrentHandleWrap::Unwrap(info.This())->pause(info[0]->IntegerValue());
    else
        TorrentHandleWrap::Unwrap(info.This())->pause();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::resume) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->resume();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::clear_error) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->clear_error();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::set_priority) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_priority(info[0]->IntegerValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::super_seeding) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->super_seeding(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::auto_managed) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->auto_managed(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::queue_position) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(TorrentHandleWrap::Unwrap(info.This())->queue_position()));
};

NAN_METHOD(TorrentHandleWrap::queue_position_up) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->queue_position_up();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::queue_position_down) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->queue_position_down();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::queue_position_top) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->queue_position_top();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::queue_position_bottom) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->queue_position_bottom();

    info.GetReturnValue().SetUndefined();
};

/*#ifndef TORRENT_DISABLE_RESOLVE_COUNTRIES
NAN_METHOD(TorrentHandleWrap::resolve_countries) {
    Nan::HandleScope scope;

    if (info.Length() == 1) {
        TorrentHandleWrap::Unwrap(info.This())->resolve_countries(info[0]->BooleanValue());
        info.GetReturnValue().SetUndefined();
    } else {
        info.GetReturnValue().Set(Nan::New<Boolean>(TorrentHandleWrap::Unwrap(info.This())->resolve_countries()));
    }
};
#endif*/

NAN_METHOD(TorrentHandleWrap::add_piece) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    std::string data(*Nan::Utf8String(info[1]));

    if (info.Length() == 3)
        th->add_piece(info[0]->IntegerValue(), data.c_str(), info[2]->IntegerValue());
    else
        th->add_piece(info[0]->IntegerValue(), data.c_str());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::read_piece) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->read_piece(info[0]->IntegerValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::have_piece) {
    Nan::HandleScope scope;

    bool ret = TorrentHandleWrap::Unwrap(info.This())->have_piece(info[0]->IntegerValue());

    info.GetReturnValue().Set(Nan::New<Boolean>(ret));
};

NAN_METHOD(TorrentHandleWrap::set_piece_deadline) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    int index = info[0]->IntegerValue(), deadline = info[1]->IntegerValue();

    if (info.Length() == 3)
        th->set_piece_deadline(index, deadline, info[2]->IntegerValue());
    else
        th->set_piece_deadline(index, deadline);

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::reset_piece_deadline) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->reset_piece_deadline(info[0]->IntegerValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::piece_availability) {
    Nan::HandleScope scope;

    std::vector<int> a;

    Local<Array> avail = info[0].As<Array>();

    for (uint32_t i(0), e(avail->Length()); i < e; ++i)
        a.push_back(avail->Get(i)->IntegerValue());

    TorrentHandleWrap::Unwrap(info.This())->piece_availability(a);

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::piece_priority) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    if (info.Length() == 2) {
        th->piece_priority(info[0]->IntegerValue(), info[1]->IntegerValue());
        info.GetReturnValue().SetUndefined();
    } else {
        info.GetReturnValue().Set(Nan::New<Integer>(th->piece_priority(info[0]->IntegerValue())));
    }
};

NAN_METHOD(TorrentHandleWrap::prioritize_pieces) {
    Nan::HandleScope scope;

    std::vector<int> p;

    Local<Array> pieces = info[0].As<Array>();

    for (uint32_t i(0), e(pieces->Length()); i < e; ++i)
        p.push_back(pieces->Get(i)->IntegerValue());

    TorrentHandleWrap::Unwrap(info.This())->prioritize_pieces(p);

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::piece_priorities) {
    Nan::HandleScope scope;

    std::vector<int> p(TorrentHandleWrap::Unwrap(info.This())->piece_priorities());

    Local<Array> ret = Nan::New<Array>();

    for (std::vector<int>::iterator i(p.begin()), e(p.end()); i != e; ++i)
        ret->Set(ret->Length(), Nan::New<Integer>(*i));

    info.GetReturnValue().Set(ret);
};

NAN_METHOD(TorrentHandleWrap::prioritize_files) {
    Nan::HandleScope scope;

    std::vector<int> f;

    Local<Array> files = info[0].As<Array>();

    for (uint32_t i(0), e(files->Length()); i < e; ++i)
        f.push_back(files->Get(i)->IntegerValue());

    TorrentHandleWrap::Unwrap(info.This())->prioritize_files(f);

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::file_priorities) {
    Nan::HandleScope scope;

    std::vector<int> f(TorrentHandleWrap::Unwrap(info.This())->file_priorities());

    Local<Array> ret = Nan::New<Array>();

    for (std::vector<int>::iterator i(f.begin()), e(f.end()); i != e; ++i)
        ret->Set(ret->Length(), Nan::New<Integer>(*i));

    info.GetReturnValue().Set(ret);
};

NAN_METHOD(TorrentHandleWrap::file_priority) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    if (info.Length() == 2) {
        th->file_priority(info[0]->IntegerValue(), info[1]->IntegerValue());
        info.GetReturnValue().SetUndefined();
    } else {
        info.GetReturnValue().Set(Nan::New<Integer>(th->file_priority(info[0]->IntegerValue())));
    }

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::use_interface) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->use_interface(*Nan::Utf8String(info[0]));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::save_resume_data) {
    Nan::HandleScope scope;

    if (info.Length() == 1)
        TorrentHandleWrap::Unwrap(info.This())->save_resume_data(info[0]->IntegerValue());
    else
        TorrentHandleWrap::Unwrap(info.This())->save_resume_data();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::need_save_resume_data) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Boolean>(TorrentHandleWrap::Unwrap(info.This())->need_save_resume_data()));
};

NAN_METHOD(TorrentHandleWrap::force_reannounce) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->force_reannounce();

    info.GetReturnValue().SetUndefined();
};

#ifndef TORRENT_DISABLE_DHT
NAN_METHOD(TorrentHandleWrap::force_dht_announce) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->force_dht_announce();

    info.GetReturnValue().SetUndefined();
};
#endif

NAN_METHOD(TorrentHandleWrap::scrape_tracker) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->scrape_tracker();

    info.GetReturnValue().SetUndefined();
};

/*NAN_METHOD(TorrentHandleWrap::name) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(TorrentHandleWrap::Unwrap(info.This())->name()).ToLocalChecked());
};*/

NAN_METHOD(TorrentHandleWrap::set_upload_mode) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_upload_mode(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::set_share_mode) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_share_mode(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::flush_cache) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->flush_cache();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::apply_ip_filter) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->apply_ip_filter(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::set_upload_limit) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    th->set_upload_limit(info[0]->Int32Value());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::set_download_limit) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    th->set_download_limit(info[0]->Int32Value());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::upload_limit) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Int32>(th->upload_limit()));
};

NAN_METHOD(TorrentHandleWrap::download_limit) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    info.GetReturnValue().Set(Nan::New<Int32>(th->download_limit()));
};

NAN_METHOD(TorrentHandleWrap::set_sequential_download) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_sequential_download(info[0]->BooleanValue());

    info.GetReturnValue().SetUndefined();
};

/*NAN_METHOD(TorrentHandleWrap::make_magnet_link) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle *handle = TorrentHandleWrap::Unwrap(info.This());
    std::string ret;

    if (!handle->is_valid()) {
        ret = "";
    } else {
        libtorrent::sha1_hash const& ih = handle->info_hash();
        ret += "magnet:?xt=urn:btih:";
        ret += libtorrent::to_hex(ih.to_string());

        libtorrent::torrent_status st = handle->status(libtorrent::torrent_handle::query_name);
        if (!st.name.empty())
        {
                ret += "&dn=";
                ret += libtorrent::escape_string(st.name.c_str(), st.name.length());
        }

        std::vector<libtorrent::announce_entry> const& tr = handle->trackers();
        for (std::vector<libtorrent::announce_entry>::const_iterator i = tr.begin(), end(tr.end()); i != end; ++i)
        {
                ret += "&tr=";
                ret +=libtorrent::escape_string(i->url.c_str(), i->url.length());
        }

        std::set<std::string> seeds = handle->url_seeds();
        for (std::set<std::string>::iterator i = seeds.begin()
                , end(seeds.end()); i != end; ++i)
        {
                ret += "&ws=";
                ret += libtorrent::escape_string(i->c_str(), i->length());
        }
    }

    info.GetReturnValue().Set(Nan::New<String>(ret).ToLocalChecked());
};*/

NAN_METHOD(TorrentHandleWrap::connect_peer) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    Local<Array> arg0 = info[0].As<Array>();

    libtorrent::tcp::endpoint ip(libtorrent::address::from_string(std::string(*Nan::Utf8String(arg0->Get(0)))),
        arg0->Get(0)->IntegerValue());

    if (info.Length() == 2)
        th->connect_peer(ip, info[1]->IntegerValue());
    else
        th->connect_peer(ip);

    info.GetReturnValue().SetUndefined();
};

/*NAN_METHOD(TorrentHandleWrap::save_path) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<String>(TorrentHandleWrap::Unwrap(info.This())->save_path()).ToLocalChecked());
};*/

NAN_METHOD(TorrentHandleWrap::set_max_uploads) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_max_uploads(info[0]->IntegerValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::max_uploads) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(TorrentHandleWrap::Unwrap(info.This())->max_uploads()));
};

NAN_METHOD(TorrentHandleWrap::set_max_connections) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_max_connections(info[0]->IntegerValue());

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::max_connections) {
    Nan::HandleScope scope;

    info.GetReturnValue().Set(Nan::New<Integer>(TorrentHandleWrap::Unwrap(info.This())->max_connections()));
};

/*NAN_METHOD(TorrentHandleWrap::set_tracker_login) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->set_tracker_login(std::string(*Nan::Utf8String(info[0])),
        std::string(*Nan::Utf8String(info[1])));

    info.GetReturnValue().SetUndefined();
};*/

NAN_METHOD(TorrentHandleWrap::move_storage) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->move_storage(std::string(*Nan::Utf8String(info[0])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::info_hash) {
    Nan::HandleScope scope;

    libtorrent::sha1_hash h(TorrentHandleWrap::Unwrap(info.This())->info_hash());

    info.GetReturnValue().Set(Nan::New<String>(libtorrent::to_hex(h.to_string())).ToLocalChecked());
};

NAN_METHOD(TorrentHandleWrap::force_recheck) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->force_recheck();

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::rename_file) {
    Nan::HandleScope scope;

    TorrentHandleWrap::Unwrap(info.This())->rename_file(info[0]->IntegerValue(),
        std::string(*Nan::Utf8String(info[1])));

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::set_ssl_certificate) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    std::string certificate(*Nan::Utf8String(info[0])),
        private_key(*Nan::Utf8String(info[1])),
        dh_params(*Nan::Utf8String(info[2]));

    if (info.Length() == 4)
        th->set_ssl_certificate(certificate, private_key, dh_params, std::string(*Nan::Utf8String(info[3])));
    else
        th->set_ssl_certificate(certificate, private_key, dh_params);

    info.GetReturnValue().SetUndefined();
};

NAN_METHOD(TorrentHandleWrap::torrent_file) {
    Nan::HandleScope scope;

    libtorrent::torrent_handle* th = TorrentHandleWrap::Unwrap(info.This());

    boost::shared_ptr<const libtorrent::torrent_info> torrent_info = th->torrent_file();

    info.GetReturnValue().Set(TorrentInfoWrap::New(torrent_info.get()));
};
