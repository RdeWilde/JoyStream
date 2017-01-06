'use strict'

const EventEmitter = require('events')

class PeerPlugin extends EventEmitter {

  constructor () {
    super()
    this.status = null

  }

  update (status) {
    this.status = status
    this.emit('statusUpdated', status)
  }

}

module.exports = PeerPlugin
