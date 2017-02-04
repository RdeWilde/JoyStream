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

  addTorrent (addTorrentParams) {

  }

  buyTorrent () {

  }

  sellTorrent () {

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
