'use strict'

var sha1 = require('sha1')
const EventEmitter = require('events')
var Peer = require('./peer')

class Torrent extends EventEmitter {

  constructor (handle, resumeData, plugin) {
    super()
    this.handle = handle
    this.resumeData = resumeData
    this.plugin = plugin
    this.torrentPlugin = null
    this.peers = new Map()
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

  addPeer(peerInfo) {

    if (!this.peers.get(peerInfo.ip)) {
      var peer = new Peer(peerInfo)
      this.peers.set(peerInfo.ip, peer)

      this.emit('peerAdded', peer)
    } else {
      debug('Is already in peers.')
    }
  }

  removePeer(peerInfo) {
    if (this.peers.get(peerInfo.ip)) {
      this.peers.delete(peerInfo.ip)
      this.emit('peerRemoved', ip)
    } else {
      debug('Not in peers list')
    }
  }

  setResumeDataGenerationResult (resumeData) {
    this.resumeData = resumeData
    this.emit('resume_data_generation_completed', resumeData)
  }

  addTorrentPlugin (torrentPluginStatus) {
    if (this.torrentPlugin) {
      debug('This torrent already have a torrentPlugin')
    } else {
      var torrentPlugin = new TorrentPlugin(torrentPluginStatus, this.plugin)
      this.emit('torrentPluginAdded', torrentPlugin)
    }
  }

  removeTorrentPlugin () {
    if (this.torrentPlugin) {
      this.torrentPlugin = null
      this.emit('torrentPluginRemoved')
    } else {
      debug('Cannot remove torrentPlugin because undefined')
    }
  }

}

module.exports = Torrent
