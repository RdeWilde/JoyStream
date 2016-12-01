var rpc = require("./rpc_client.js");
var express = require('express');
var app = express();

var torrent = {
	'type': 'INFOHASH',
	'infohash': 'd59e6da0de8f5382f067e07375c262f15570a8f1'
}

var torrentFilePath = '306497171.torrent'

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

app.listen(3000, function () {
  console.log('Example app listening on port 3000!');

	/*rpc.receivedAddress(function(err, bitcoinAddress) {
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
	})*/

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

			rpc.getTorrentState(torrent, function(err, torrentState) {
				if (err) {
					console.log(err)
				} else {
					console.log('Torrent State :', torrentState)
				}
			})

			rpc.buyTorrent(buyTorrentRequest, function(err, answer) {
					if (err) {
						console.log(err)
					} else {
						console.log('We are buying the torrent')
					}
			})
		}
	})

	/*rpc.addTorrent(torrent, function(err, torrentAdded) {
		if (err) {
			console.log(err);
		} else {
			console.log(torrentAdded);

			rpc.getTorrentState(torrentAdded, function(err, torrentState) {
				if (err) {
					console.log(err)
				} else {
					console.log('Torrent State :', torrentState)
				}
			})

			rpc.buyTorrent(torrentAdded, function(err, answer) {
					if (err) {
						console.log(err)
					} else {
						console.log('We are buying the torrent')
					}
			})

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
				})
			})
		}
	})*/
})
