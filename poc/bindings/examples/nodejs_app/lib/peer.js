'use strict'

const EventEmitter = require('events')

class Peer extends EventEmitter {

  constructor () {
    super()
    this.plugin = null

  }

}

module.exports = Peer
