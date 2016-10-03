var rpc = require("./rpc_client.js");

var torrent = {
	'infohash': '6a9759bffd5c0af65319979fb7832189f4f3c35d',
	'name': 'Sintel'
}

rpc.addTorrent(torrent, function(err, torrent) {
	if (err) {
		console.log(err);
	} else {
		console.log(torrent);

		rpc.listTorrents(function(err, torrent){
			if(err){
				console.log(err);
			} else {
				console.log(torrent);
			}
		}, function(){
			console.log("Got All Torrents");
			rpc.pause(function(){
				console.log('Daemon stop')
			});
		});
	}
});
