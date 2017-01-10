'use strict'

const EventEmitter = require('events')

class TorrentPlugin extends EventEmitter {

  constructor (infoHash, status, plugin) {
    super()
    this.status = status
    this.plugin = plugin
    this.infoHash = status.infoHash()
  }

  update (status) {
    this.emit('statusUpdated', status)
  }

}

module.exports = TorrentPlugin
