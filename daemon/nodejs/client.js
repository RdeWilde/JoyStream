var PROTO_PATH = __dirname + '/../protos/daemon.proto';

var grpc = require('grpc');

var rpc = grpc.load(PROTO_PATH).joystream.daemon.rpc;

var client = new rpc.Daemon('localhost:30000', grpc.credentials.createInsecure());

client.pause({}, function(){

});
