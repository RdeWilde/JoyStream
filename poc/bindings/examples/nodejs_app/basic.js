var Node = require('./lib/node')
var debug = require('debug')('basic')

debug('Starting basic.js example')

var node = new Node()

node.pause()

console.log(node.isPaused())

node.resume()

console.log(node.isPaused())

console.log(node.listenPort())

node.addTorrent(0, 0, 'Sintel', null, '/home/lola/joystream/test/', '6a9759bffd5c0af65319979fb7832189f4f3c35d',
  function (err, smth) {
    if (err) {
      debug(err)
    } else {
      console.log('This has been called once the torrent is added')
    }
  })
