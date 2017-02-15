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
    this.plugin.add_torrent(addTorrentParams, (err, torrentHandle) => {
      if (!err) {
        var torrent = this.torrents.get(torrentHandle.infoHash())
        // Verify if torrent not already in torrents list
        if (!torrent) {

          var torrent = new Torrent(torrentHandle,
                                    '',
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
      } else {
        // Need error wrapper for message
        debug('Adding torrent failed:')
      }
      callback(err, torrentHandle)
    })
  }

  buyTorrent (infoHash, buyerTerms, callback) {
    this.plugin.to_buy_mode(infoHash, buyerTerms, callback)
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
