var lib = require('../../')
var debug = require('debug')('basic')

debug('Starting basic.js example')

var app = new lib.Joystream()

console.log(app.isPaused())

console.log(app.listenPort())

/*let addTorrentParams = {
  infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
  name: '306497171',
  path: '/home/lola/joystream/test/'
}*/

let addTorrentParams = {
  ti: new lib.TorrentInfo('/home/lola/joystream/test/306497171.torrent'),
  savePath: '/home/lola/joystream/test/'
}

/*let addTorrentParams = {
  url: 'magnet:?xt=urn:btih:6a9759bffd5c0af65319979fb7832189f4f3c35d&dn=sintel.mp4&tr=udp%3A%2F%2Fexodus.desync.com%3A6969&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80&tr=wss%3A%2F%2Ftracker.btorrent.xyz&tr=wss%3A%2F%2Ftracker.fastcast.nz&tr=wss%3A%2F%2Ftracker.openwebtorrent.com&tr=wss%3A%2F%2Ftracker.webtorrent.io&ws=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2Fsintel-1024-surround.mp4',
  path: '/home/lola/joystream/test/'
}*/


app.addTorrent(addTorrentParams, (err, torrentHandle) => {
  console.log('Torrent Added to plugin !')
})
