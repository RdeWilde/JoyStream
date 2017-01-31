'use strict'

const EventEmitter = require('events')
const PeerPlugin = require('./peerPlugin')

class Peer extends EventEmitter {

  constructor (info, plugin = null) {
    super()
    this.peerInformation = info
    this.peerPlugin = plugin
  }

  addPeerPlugin (status) {
    var peerPlugin = new PeerPlugin(status)
    this.emit('peerPluginAdded', peerPlugin)
  }

  removePeerPlugin () {
    this.peerPlugin = null
    this.emit('peerPluginRemoved')
  }

}

module.exports = Peer
