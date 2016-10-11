var PROTO_PATH = __dirname + '/../protos/daemon.proto';

var grpc = require('grpc');

var rpc = grpc.load(PROTO_PATH).joystream.daemon.rpc;

var clientCallback = new rpc.Callback('localhost:3002', grpc.credentials.createInsecure());
var clientDaemon = new rpc.Daemon('localhost:3002', grpc.credentials.createInsecure());

var rpc = module.exports;

rpc.pause = function(callback){
  clientDaemon.pause({}, callback);
};

rpc.listTorrents = function(data, done ) {
  var call = clientDaemon.listTorrents();

  call.on('data', function(torrent){
    data(null, torrent);
  });

  call.on('end',function(){
    done();
  });

  call.on('error', function(err){
  	data(err);
  });
}

rpc.torrentAdded = function(callback) {
  clientCallback.torrentAdded({}, callback);
};

rpc.addTorrent = function(torrent, callback ) {
  // we prepare the async callback request
  rpc.torrentAdded(function(err, torrent) {
    if (err) {
      console.log('Something went wrong')
    } else {
      console.log('A torrent has been added')
      console.log(torrent)
    }
  });
  clientDaemon.addTorrent(torrent, callback);
}
