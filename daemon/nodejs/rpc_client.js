var PROTO_PATH = __dirname + '/../protos/daemon.proto';

var grpc = require('grpc');

var rpc = grpc.load(PROTO_PATH).joystream.daemon.rpc;

var clientHello = new rpc.Hello('localhost:3002', grpc.credentials.createInsecure());
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

rpc.addTorrent = function(torrent, callback ) {
  clientDaemon.addTorrent(torrent, callback);
}

rpc.sayHello = function(callback) {
  console.log('Hello');
  clientHello.sayHello({}, callback);
};
