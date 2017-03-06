'use strict'

const Node = require('./node')
const Torrent = require('./torrent')
const SPVWallet = require('./SPVWallet')
const StateT = require('./StateT')
var debug = require('debug')('joystream')

/**
 * Joystream class.
 * Manage the application
 */
class Joystream extends Node {

  constructor (options) {
    super(options.port)
    // Init spvwallet
    this.wallet = new SPVWallet(options.wallet)
  }

  buyTorrent (infoHash, buyerTerms, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      if (torrent.torrentPlugin) {
        if (torrent.handle.status().state === StateT.DOWNLOADING ) {
          /*
            TODO:
            - verify fund in wallet
            - Lock minimum funds
            - Get TorrentPlugin
            - Need paychanKeysGenerator, receiveAddressesGenerator, changeAddressesGenerator ?
           */

          this.plugin.to_buy_mode(infoHash, buyerTerms, callback)
        } else {
          debug('Torrent not in downloading state')
          callback(new Error('Torrent not in downloading state'), null)
        }
      } else {
        debug('TorrentPlugin not set for this torrent')
        callback(new Error('TorrentPlugin not set for this torrent'), null)
      }
    } else {
      debug('Torrent not present in node !')
      callback(new Error('Torrent not present in node !'), null)
    }
  }

  // If torrent not find try to add ?
  sellTorrent (infoHash, sellerTerms, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      if (torrent.torrentPlugin) {
        if (torrent.handle.status().state === StateT.SEEDING) {
          this.plugin.to_sell_mode(infoHash, sellerTerms, callback)
        } else {
          debug('Torrent not in seeding state')
          callback(new Error('Torrent not in seeding state'), null)
        }
      } else {
        debug('TorrentPlugin not set for this torrent')
        callback(new Error('TorrentPlugin not set for this torrent'), null)
      }
    } else {
      debug('Torrent not present in node !')
      // Add torrent to node ?
      callback(new Error('Torrent not present in node !'), null)
    }
  }

  observeTorrent () {

  }

  resumeTorrent () {

  }

  pauseTorrent () {

  }

  generateSavedTorrents () {
    // Use levelDB instead ?
  }

  pauseLibtorrent (callback) {
    this.plugin.pause_libtorrent((err, result) => {
      debug('Libtorrent paused succesfully')
      if (!err)
        callback(err, result)
    })
  }

  syncWallet () {

  }

  getAddress () {
    return this.wallet.getAddress()
  }

  getBalance () {
    return this.wallet.getBalance()
  }

  shutdown () {

  }

}

module.exports = Joystream