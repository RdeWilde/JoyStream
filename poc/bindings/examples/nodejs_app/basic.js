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

console.log(app.isPaused())

console.log(app.listenPort())

app.wallet.start().then(() => {

  debug('Wallet Ready we can start to sell')

  debug('Address to fund wallet : ', app.getAddress().toString())

  /*let addTorrentParams = {
    infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
    name: 'sintel.mp4',
    path: '/home/lola/joystream/test/'
  }*/

  let addTorrentParams = {
    ti: new lib.TorrentInfo('/home/lola/joystream/test/306497171.torrent'),
    savePath: '/home/lola/joystream/test/'
  }

  /*let addTorrentParams = {
    url: 'magnet:?xt=urn:btih:6a9759bffd5c0af65319979fb7832189f4f3c35d&dn=sintel.mp4&tr=udp%3A%2F%2Fexodus.desync.com%3A6969&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80&tr=wss%3A%2F%2Ftracker.btorrent.xyz&tr=wss%3A%2F%2Ftracker.fastcast.nz&tr=wss%3A%2F%2Ftracker.openwebtorrent.com&tr=wss%3A%2F%2Ftracker.webtorrent.io&ws=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2Fsintel-1024-surround.mp4',
    path: '/home/lola/joystream/test/'
  }*/

  app.addTorrent(addTorrentParams, (err, torrent) => {

    debug('Torrent Added !')

    if (!err) {

      //50, 1, 10, 15000, 5000
      let sellerTerm = {
        minPrice: 50,
        minLock: 1,
        maxNumberOfSellers: 10,
        minContractFeePerKb: 15000,
        settlementFee: 5000
      }

      //100, 5, 1, 20000
      let buyerTerm = {
        maxPrice: 100,
        maxLock: 5,
        minNumberOfSellers: 1,
        maxContractFeePerKb: 20000
      }

      // we wait for the plugin to be added
      torrent.on('torrentPluginAdded', () => {
        debug('Torrent Plugin added')

        torrent.on('state_changed_alert', () => {
          if (torrent.handle.status().state === 5) {
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
