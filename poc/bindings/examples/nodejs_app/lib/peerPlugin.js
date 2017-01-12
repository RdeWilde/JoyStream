'use strict'

const EventEmitter = require('events')

class PeerPlugin extends EventEmitter {

  constructor (status) {
    super()
    this.status = status

  }

  update (status) {
    this.status = status
    this.emit('statusUpdated', status)
  }

}

module.exports = PeerPlugin
