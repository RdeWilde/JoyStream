'use strict'

const Node = require('./node')
const Torrent = require('./torrent')
const SPVWallet = require('./SPVWallet')
const debug = require('debug')('joystream')

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
