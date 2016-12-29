'use strict'

var sha1 = require('sha1')
const EventEmitter = require('events')

class Torrent extends EventEmitter {

  constructor (handle, resumeData, plugin) {
    super()
    this.handle = handle
    this.resumeData = resumeData
    this.plugin = plugin
    this.announcedJSPeersAtTimestamp = new Map()
  }

  // Review needed !
  secondaryInfoHash () {
    var newInfoHash = this.handle.infoHash() + '_JS'
    return sha1(newInfoHash)
  }

  addJSPeerAtTimestamp (address, timestamp) {
    this.announcedJSPeersAtTimestamp.set(address, timestamp)
  }

  addPeer(ip) {
    // TODO
  }

  removePeer(ip) {
    // TODO
  }

}

module.exports = Torrent
