var PROTO_PATH = __dirname + '/../protos/daemon.proto';

var grpc = require('grpc');

var rpc = grpc.load(PROTO_PATH).joystream.daemon.rpc;

var clientDaemon = new rpc.Daemon('localhost:3002', grpc.credentials.createInsecure());
var wallet = new rpc.Wallet('localhost:3002', grpc.credentials.createInsecure());

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

rpc.addTorrent = function(torrent, callback ) {
  clientDaemon.addTorrent(torrent, callback);
}

rpc.removeTorrent = function(torrent, callback ) {
  clientDaemon.removeTorrent(torrent, callback);
}

rpc.pauseTorrent = function(torrent, callback ) {
  clientDaemon.pauseTorrent(torrent, callback);
}

rpc.startTorrent = function(torrent, callback ) {
  clientDaemon.startTorrent(torrent, callback);
}

rpc.receivedAddress = function(callback ) {
  wallet.receivedAddress({}, callback);
}

rpc.balance = function(callback ) {
  wallet.balance({}, callback);
}

rpc.unconfirmedBalance = function(callback ) {
  wallet.unconfirmedBalance({}, callback);
}
