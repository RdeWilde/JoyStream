var rpc = require("./rpc_client.js");
var express = require('express');
var app = express();

var torrent = {
	'infohash': '6a9759bffd5c0af65319979fb7832189f4f3c35d',
	'name': 'Sintel'
}

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

	rpc.receivedAddress(function(err, bitcoinAddress) {
		if(err) {
			console.log(err)
		} else {
			console.log('Received BitcoinAddress to move fund :',bitcoinAddress)
		}
	})

	rpc.balance(function(err, BTCAmount) {
		if(err) {
			console.log(err)
		} else {
			console.log('Your current Balance:',BTCAmount)
		}
	})

	rpc.addTorrent(torrent, function(err, torrentAdded) {
		if (err) {
			console.log(err);
		} else {
			console.log(torrentAdded);

			rpc.listTorrents(function(err, torrentRecieved) {
				if (err) {
					console.log(err)
				} else {
					console.log(torrentRecieved);
				}
			}, function() {
				rpc.startTorrent(torrentAdded, function(err, answer) {
					if (err) {
						console.log(err)
					} else {
						console.log('Torrent restarted')

						rpc.pauseTorrent(torrentAdded, function(err, answer) {
							if (err) {
								console.log('Something wrong happened')
							} else {
								console.log('Empty answer : ', answer)

								/*rpc.removeTorrent(torrent, function(err, torrentState){
									if (err) {
										console.log(err);
									} else {
										console.log(torrentState);
									}
								});*/
							}
						})
					}
				})
			})
		}
	})
})
