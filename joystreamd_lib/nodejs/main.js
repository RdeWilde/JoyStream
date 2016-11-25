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

	rpc.unconfirmedBalance(function(err, BTCAmount) {
		if(err) {
			console.log(err)
		} else {
			console.log('Your current unconfirmed Balance:',BTCAmount)
		}
	})

	rpc.status(function(err, WalletStatus) {
		if(err) {
			console.log(err)
		} else {
			console.log('WalletStatus :',WalletStatus)
		}
	})

	rpc.suscribeStatus(function(err, WalletStatus) {
		if (err) {
			console.log(err)
		} else {
			console.log(WalletStatus)
		}
	}, function() {
		console.log('Call Terminated !')
	})

	rpc.suscribeEvents(function(err, Event) {
		if (err) {
			console.log(err)
		} else {
			console.log(Event)
		}
	}, function() {
		console.log('Call Terminated')
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

				rpc.removeTorrent(torrent, function(err, torrentState){
					if (err) {
						console.log(err);
					} else {
						console.log(torrentState);

						rpc.listTorrents(function(err, torrentRecieved) {
							if (err) {
								console.log(err)
							} else {
								console.log(torrentRecieved);
							}
						}, function() {
							console.log('Done !')
						})
					}
				});

				/*rpc.buyTorrent(torrentAdded, function(err, answer) {
						if (err) {
							console.log(err)
						} else {
							console.log('We are buying the torrent')
						}
					})

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
							}
						})
					}
				})*/
			})
		}
	})
})
