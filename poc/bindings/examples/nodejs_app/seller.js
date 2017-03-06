var lib = require('../../')
var debug = require('debug')('seller')

var app = new lib.Joystream({
  port: 6881,
  wallet: {
    db: 'leveldb',
    prefix: '/home/lola/joystream/test/',
    network: 'testnet',
    httpPort: 18332
  }
})

app.wallet.start().then(() => {
  debug('Wallet Ready !')

  let addTorrentParams = {
    ti: new lib.TorrentInfo('/home/lola/joystream/test/306497171.torrent'),
    savePath: '/home/lola/joystream/test/'
  }

  app.addTorrent(addTorrentParams, (err, torrent) => {
    debug('Torrent Added !')

    if (!err) {
      //  50, 1, 10, 15000, 5000
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
          debug('Torrent state changed')
          if (torrent.handle.status().state === 5) {
            debug('Torrent seeding, we can go to sell mode')
            app.sellTorrent('d59e6da0de8f5382f067e07375c262f15570a8f1', sellerTerm, (err, result) => {
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
