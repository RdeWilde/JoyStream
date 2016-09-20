var rpc = require("./rpc_client.js");

rpc.listTorrents(function(err, torrent){
	if(err){
		console.log(err);
	} else {
		console.log(torrent);
	}
}, function(){
	console.log("Got All Torrents");
	rpc.pause(function(){

	});
});


