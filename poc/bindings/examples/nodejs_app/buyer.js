var lib = require('../../')
var debug = require('debug')('buyer')

var app = new lib.Joystream({
  port: 6882,
  wallet: {
    db: 'leveldb',
    prefix: '/home/lola/joystream/test2/',
    network: 'testnet',
    httpPort: 18333
  }
})

let addTorrentParams = {
  infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
  name: 'sintel.mp4',
  path: '/home/lola/joystream/test/'
}

app.wallet.start().then(() => {
  debug('Wallet Ready !')

  debug('Address to fund wallet : ', app.getAddress().toString())

  app.getBalance().then((balance) => {
    debug('Balance : ', balance )
  })

  app.addTorrent(addTorrentParams, (err, torrent) => {
    debug('Torrent Added !')

    if (!err) {
      //  100, 5, 1, 20000
      let buyerTerm = {
        maxPrice: 100,
        maxLock: 5,
        minNumberOfSellers: 1,
        maxContractFeePerKb: 20000
      }

      // we wait for the plugin to be added
      torrent.on('torrentPluginAdded', () => {
        debug('Torrent Plugin added')

        /*
          Buy Torrent start here
        */


        // Verify if it is on downloading status
        if (torrent.handle.status().state === 2) {
          torrent.toBuyMode(buyerTerm, (err, result) => {
            if (!err) {
              debug('We are in buying mode')
            } else {
              console.log(err)
            }
          })
        } else {
          // Wait to pass to change status
          torrent.on('state_changed_alert', () => {
            debug('Torrent state changed')
            // Verify if is downloading state
            if (torrent.handle.status().state === 2) {
              debug('Torrent seeding, we can go to sell mode')
              torrent.toBuyMode(buyerTerm, (err, result) => {
                if (!err) {
                  debug('We are in buying mode')
                } else {
                  console.log(err)
                }
              })
            }
          })
        }
      })
    } else {
      debug(err)
    }
  })
})
