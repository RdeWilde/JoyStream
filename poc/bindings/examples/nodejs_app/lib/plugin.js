'use strict'

const EventEmitter = require('events')

class Plugin extends EventEmitter {

  constructor () {
    super()

  }

  update (status) {
    this.emit('TorrentPluginStatusUpdateAlert', status)
  }

}

module.exports = Plugin
