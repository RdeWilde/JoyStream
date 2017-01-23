#ifndef TORRENT_INFO_H
#define TORRENT_INFO_H

#include <nan.h>
#include <boost/optional.hpp>
#include <libtorrent/torrent_info.hpp>

using namespace v8;

class TorrentInfo: public Nan::ObjectWrap {
    public:
      static NAN_MODULE_INIT(Init);
      static Local<Object> New(boost::shared_ptr<const libtorrent::torrent_info>& ti);
      static const libtorrent::torrent_info* Unwrap(const Local<Object>& obj) {
        TorrentInfo* ti = Nan::ObjectWrap::Unwrap<TorrentInfo>(obj);
        return ti->torrent_info_.get();
      };

    private:
      boost::shared_ptr<const libtorrent::torrent_info> torrent_info_;
      static Nan::Persistent<Function> constructor;

      static NAN_METHOD(NewInstance);
      /*static NAN_METHOD(remap_files);
      static NAN_METHOD(add_tracker);
      static NAN_METHOD(add_url_seed);
      static NAN_METHOD(add_http_seed);
      static NAN_METHOD(web_seeds);
      static NAN_METHOD(name);
      static NAN_METHOD(comment);
      static NAN_METHOD(creator);
      static NAN_METHOD(total_size);
      static NAN_METHOD(piece_length);
      static NAN_METHOD(num_pieces);
      static NAN_METHOD(info_hash);
      static NAN_METHOD(hash_for_piece);
      static NAN_METHOD(merkle_tree);
      static NAN_METHOD(set_merkle_tree);
      static NAN_METHOD(piece_size);
      static NAN_METHOD(num_files);
      static NAN_METHOD(file_at);
      static NAN_METHOD(files);
      static NAN_METHOD(orig_files);
      static NAN_METHOD(rename_file);
      static NAN_METHOD(priv);
      static NAN_METHOD(trackers);
      static NAN_METHOD(creation_date);
      static NAN_METHOD(add_node);
      static NAN_METHOD(nodes);
      static NAN_METHOD(metadata);
      static NAN_METHOD(metadata_size);
      static NAN_METHOD(map_block);
      static NAN_METHOD(map_file);*/
};

#endif
