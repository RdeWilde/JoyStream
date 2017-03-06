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
  infoHash: 'd59e6da0de8f5382f067e07375c262f15570a8f1',
  path: '/home/lola/joystream/test2/'
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

      console.log(torrent.handle.infoHash() === addTorrentParams.infoHash)

      // we wait for the plugin to be added
      torrent.on('torrentPluginAdded', () => {
        debug('Torrent Plugin added')

        /*
          Buy Torrent start here
        */


        // Verify if it is on downloading status
        if (torrent.handle.status().state === 2) {
          app.buyTorrent('d59e6da0de8f5382f067e07375c262f15570a8f1', buyerTerm, (err, result) => {
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
              app.buyTorrent('d59e6da0de8f5382f067e07375c262f15570a8f1', buyerTerm, (err, result) => {
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
