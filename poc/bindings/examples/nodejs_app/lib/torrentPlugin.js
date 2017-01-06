'use strict'

const EventEmitter = require('events')

class TorrentPlugin extends EventEmitter {

  constructor () {
    super()

  }

  update (status) {
    this.emit('statusUpdated', status)
  }

}

module.exports = TorrentPlugin
