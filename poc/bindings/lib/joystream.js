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
    super()
    // Init spvwallet
    options.db = 'leveldb'
    this.wallet = new SPVWallet(options)

    this.wallet.start().then(() => {
      debug('Wallet ready to use')
      console.log(this.wallet.getAddress().toString())
      console.log(this.wallet.getAllAddresses().then((addresses) => {
        console.log(addresses.toString())
        console.log(this.wallet.getAddress().toString())
      }))
    })
  }

  addTorrent (addTorrentParams, callback) {
    this.plugin.add_torrent(addTorrentParams, (err, torrentHandle) => {
      if (!err) {
        var torrent = this.torrents.get(torrentHandle.infoHash())
        // Verify if torrent not already in torrents list
        if (!torrent) {

          var torrent = new Torrent(torrentHandle,
                                    null,
                                    this.plugin)
          // Add torrent to torrents map
          this.torrents.set(torrentHandle.infoHash(),torrent)
          // Emit event 'addTorrentAlert'
          this.emit('add_torrent_alert', torrent)

          // DHT stuff
          this.torrentsBySecondaryHash.set(torrent.secondaryInfoHash(), torrentHandle.infoHash())
          this.session.dhtAnnounce(torrent.secondaryInfoHash(), this.listenPort())

        } else {
          torrent.resumeData = resumeData
        }
        debug('Adding torrent succeeded.')
        callback(err, torrent)
      } else {
        // Need error wrapper for message
        debug('Adding torrent failed')
        callback(err, null)
      }
    })
  }

  removeTorrent (infoHash, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      this.plugin.remove_torrent(infoHash, (err, result) => {
        if (!err)
          this.torrents.delete(infoHash)
      })
    } else {
      debug('Cannot remove torrent : Torrent not found')
      callback(new Error('Cannot remove torrent : Torrent not found'), null)
    }

  }

  buyTorrent (infoHash, buyerTerms, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      if (torrent.torrentPlugin) {
        if (torrent.handle.status() === StateT.DOWNLOADING ) {
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
      // Add torrent to node ?
      callback(new Error('Torrent not present in node !'), null)
    }
  }

  // If torrent not find try to add ?
  sellTorrent (infoHash, sellerTerms, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      if (torrent.torrentPlugin) {
        if (torrent.handle.status() === StateT.SEEDING) {
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
