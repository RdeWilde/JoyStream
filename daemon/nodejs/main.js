var rpc = require("./rpc_client.js");
var express = require('express');
var app = express();

var torrent = {
	'infohash': '6a9759bffd5c0af65319979fb7832189f4f3c35d',
	'name': 'Sintel'
}

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');


	rpc.addTorrent(torrent, function(err) {
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

});
