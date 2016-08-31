var PROTO_PATH = __dirname + '/../protos/daemon.proto';

var grpc = require('grpc');

var rpc = grpc.load(PROTO_PATH).joystream.daemon.rpc;

var client = new rpc.Daemon('localhost:30000', grpc.credentials.createInsecure());

var rpc = module.exports;

rpc.pause = function(callback){

  client.pause({}, callback);

};

rpc.listTorrents = function(data, done ) {
  var call = client.listTorrents();

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
