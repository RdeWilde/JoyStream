'use strict'

const EventEmitter = require('events')
const PeerPlugin = require('./peerPlugin')

class Peer extends EventEmitter {

  constructor (info, status = null) {
    super()
    this.peerInformation = info
    this.peerPlugin = status ? new PeerPlugin(status) : null
  }

  addPeerPlugin (status) {
    var peerPlugin = new PeerPlugin(status)
  }

  removePeerPlugin () {
    this.peerPlugin = null
  }

}

module.exports = Peer
