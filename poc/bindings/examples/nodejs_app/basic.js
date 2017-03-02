var lib = require('../../')
var debug = require('debug')('basic')

debug('Starting basic.js example')

var app = new lib.Joystream({
    port: 6882,
    wallet: {
      db: 'leveldb',
      prefix  : '/home/lola/joystream/test/',
      network : 'testnet',
      httpPort: 18332
    }
  })

app.wallet.start().then(() => {

  debug('Wallet Ready we can start to sell')

  debug('Address to fund wallet : ', app.getAddress().toString())

  let addTorrentParams = {
    infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
    path: '/home/lola/joystream/test/'
  }

  app.addTorrent(addTorrentParams, (err, torrent) => {
    if (!err) {

      debug('Torrent Added !')

      //50, 1, 10, 15000, 5000
      let sellerTerm = {
        minPrice: 50,
        minLock: 1,
        maxNumberOfSellers: 10,
        minContractFeePerKb: 15000,
        settlementFee: 5000
      }

      // we wait for the plugin to be added
      torrent.on('torrentPluginAdded', () => {
        debug('Torrent Plugin added')

        torrent.on('state_changed_alert', () => {
          if (torrent.handle.status().state === 5) {
            torrent.toSellMode(sellerTerm, (err, result) => {
              if (!err) {
                debug('We are in selling mode')
              } else {
                console.log(err)
              }
            })
          }
        })
      })
    } else {
      debug(err)
    }
  })
})
