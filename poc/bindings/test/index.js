var lib = require('../')
var assert = require('assert')
var _ = require('lodash')

describe('JoyStream addon', function() {
  describe('Adding torrent to plugin', function() {
    it('Add torrent with torrent info file', function() {
      let addTorrentParams = {
        ti: new lib.TorrentInfo(__dirname + '/sintel.torrent'),
        savePath: __dirname
      }
      var app = new lib.Joystream({
        port: 6881,
        wallet: {
          db: 'leveldb',
          prefix: __dirname ,
          network: 'testnet',
          httpPort: 18332
        }
      })

      app.addTorrent(addTorrentParams, (err, torrent) => {
        assert(!err)
      })
    })
    it('Add torrent with magnet url', function() {
      let addTorrentParams = {
        url: 'magnet:?xt=urn:btih:6a9759bffd5c0af65319979fb7832189f4f3c35d&dn=sintel.mp4&tr=udp%3A%2F%2Fexodus.desync.com%3A6969&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.internetwarriors.net%3A1337&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Ftracker.openbittorrent.com%3A80&tr=wss%3A%2F%2Ftracker.btorrent.xyz&tr=wss%3A%2F%2Ftracker.fastcast.nz&tr=wss%3A%2F%2Ftracker.openwebtorrent.com&tr=wss%3A%2F%2Ftracker.webtorrent.io&ws=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2Fsintel-1024-surround.mp4',
        path: __dirname
      }
      var app = new lib.Joystream({
        port: 6881,
        wallet: {
          db: 'leveldb',
          prefix: __dirname ,
          network: 'testnet',
          httpPort: 18332
        }
      })

      app.addTorrent(addTorrentParams, (err, torrent) => {
        assert(!err)
      })
    })
    it('Add torrent with info_hash', function() {
      let addTorrentParams = {
        infoHash: '6a9759bffd5c0af65319979fb7832189f4f3c35d',
        name: 'sintel.mp4',
        path: __dirname
      }
      var app = new lib.Joystream({
        port: 6881,
        wallet: {
          db: 'leveldb',
          prefix: __dirname ,
          network: 'testnet',
          httpPort: 18332
        }
      })
      app.addTorrent(addTorrentParams, (err, torrent) => {
        assert(!err)
      })
    })
  })
})
