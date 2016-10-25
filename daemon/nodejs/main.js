var rpc = require("./rpc_client.js");
var express = require('express');
var app = express();

var torrent = {
	'infohash': '6a9759bffd5c0af65319979fb7832189f4f3c35d',
	'name': 'Sintel'
}

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

	rpc.test1({clientMessage: 'Hello'}, function(err, responce) {
		if (err) {
			console.log(err)
		} else {
			console.log(responce);
			// Pause the Node
			rpc.pause(function() {
				console.log('Node is paused')
			});
		}
	})
});
