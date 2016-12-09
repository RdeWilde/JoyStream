var rpc = require("./rpc_client2.js");
var express = require('express');
var app = express();

var fs = require('fs')

var buffer = fs.readFileSync('306497171.torrent')

var torrent = {
	'type': 2,
	'data': buffer
}

var buyTorrentRequest = {
	'infohash': 'd59e6da0de8f5382f067e07375c262f15570a8f1',
	'nsellers': 1,
	'price': 100,
	'locktime': 5,
	'settlement_fee': 5000,
	'contractFeeRate': 20000,
	'secondsBeforeCreatingContract': 3,
	'secondsBeforePieceTimeout': 25
}

app.listen(3001, function () {
  console.log('Example app listening on port 3001!');

	rpc.balance(function(err, BTCAmount) {
		if(err) {
			console.log(err)
		} else {
			console.log('Your current Balance:',BTCAmount)
		}
	})

	rpc.unconfirmedBalance(function(err, BTCAmount) {
		if(err) {
			console.log(err)
		} else {
			console.log('Your current unconfirmed Balance:',BTCAmount)
		}
	})

	rpc.addTorrent(torrent, function(err, data) {
		if (err) {
			console.log(err)
		} else {
			console.log('Torrent added')

			rpc.listTorrents(function(err, torrentRecieved) {
				if (err) {
					console.log(err)
				} else {
					console.log(torrentRecieved);
				}
			}, function() {})

			rpc.buyTorrent(buyTorrentRequest, function(err, answer) {
					if (err) {
						console.log(err)
					} else {
						console.log('We are buying the torrent')
					}
			})
		}
	})

});
