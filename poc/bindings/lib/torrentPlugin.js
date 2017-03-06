'use strict'

const EventEmitter = require('events')

class TorrentPlugin extends EventEmitter {

  constructor (status, plugin) {
    super()
    this.status = status
    this.plugin = plugin
  }

  update (status) {
    this.emit('statusUpdated', status)
  }

}

module.exports = TorrentPlugin
