'use strict'

const EventEmitter = require('events')

class Plugin extends EventEmitter {

  constructor () {
    super()

  }

  update (status) {
    this.emit('statusUpdated', status)
  }

}

module.exports = Plugin
