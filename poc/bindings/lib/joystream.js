'use strict'

const Node = require('./node')
const SPVWallet = require('./SPVWallet')

/**
 * Joystream class.
 * Manage the application
 */
class Joystream extends Node {

  constructor (options) {
    super()
    // Init spvwallet
    this.wallet = new SPVWallet(options)

    this.wallet.start()
  }

  addTorrent (addTorrentParams, callback) {
    this.plugin.add_torrent(addTorrentParams, (err, result) => {
      if (!err)
        console.log(result)
      callback(err, result)
    })
  }

  buyTorrent () {

  }

  // If torrent not find try to add ?
  sellTorrent (infoHash, sellerTerms, callback) {
    this.plugin.to_sell_mode(infoHash, sellerTerms, callback)
  }

  observeTorrent () {

  }

  startTorrent () {

  }

  stopTorrent () {

  }

  generateSavedTorrents () {

  }

  syncWallet () {

  }

  getBalance () {
    this.wallet.getBalance()
  }

  shutdown () {

  }

}

module.exports = Joystream
