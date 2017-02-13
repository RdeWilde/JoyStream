var Joystream = require('./dist/joystream')
var NativeExtension = require('bindings')('NativeExtension')

module.exports = {
  TorrentInfo: NativeExtension.TorrentInfo,
  Joystream
}
