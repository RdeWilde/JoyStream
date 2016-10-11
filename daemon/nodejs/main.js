var rpc = require("./rpc_client.js");
var express = require('express');
var app = express();

var torrent = {
	'infohash': '6a9759bffd5c0af65319979fb7832189f4f3c35d',
	'name': 'Sintel'
}

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

	rpc.listTorrents(function(err, t){
		if(err){
			console.log(err);
		} else {
			console.log('This torrent is in the node',t);
		}
	}, function(){
		console.log("Got All Torrents");
	});

	rpc.addTorrent(torrent, function(err) {
		if (err) {
			console.log(err);
		} else {
			console.log('The torrent is being added, waiting for callback...')
		}
	});

});
