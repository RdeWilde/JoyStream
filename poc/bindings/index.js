var Joystream = require('./dist/joystream')
var StateT = require('./dist/StateT')
var NativeExtension = require('bindings')('NativeExtension')

module.exports = {
  TorrentInfo: NativeExtension.TorrentInfo,
  Joystream,
  StateT
}
