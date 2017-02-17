'use strict'

const Node = require('./node')
const Torrent = require('./torrent')
const SPVWallet = require('./SPVWallet')
var debug = require('debug')('joystream')

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

  buyTorrent (infoHash, buyerTerms, callback) {
    var torrent = this.torrents.get(infoHash)

    if (torrent) {
      if (torrent.torrentPlugin) {
        if (torrent.handle.status()) {
          console.log(torrent.handle.status())
          this.plugin.to_buy_mode(infoHash, buyerTerms, callback)
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
        if (torrent.handle.status()) {
          console.log(torrent.handle.status())
          this.plugin.to_sell_mode(infoHash, sellerTerms, callback)
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
