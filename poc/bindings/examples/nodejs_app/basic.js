var Joystream = require('../../dist/joystream')
var debug = require('debug')('basic')

debug('Starting basic.js example')

var app = new Joystream()

console.log(app.isPaused())

console.log(app.listenPort())

let addTorrentParams = {
  infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
  name: '306497171',
  path: '/home/lola/joystream/test/'
}

app.addTorrent(addTorrentParams, (err, torrentHandle) => {
  console.log('Torrent Added to plugin !')
})
