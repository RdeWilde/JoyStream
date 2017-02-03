'use strict'

const path = require('path')
const Node = require('./node')

var appDir = path.dirname(require.main.filename)

/**
 * Joystream class.
 * Manage the application
 */
class Joystream extends Node {

  constructor (path = appDir, network='Testnet') {
    super()
    // Init spvwallet
    this.wallet = null
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

  shutdown () {

  }

}

module.exports = Joystream
