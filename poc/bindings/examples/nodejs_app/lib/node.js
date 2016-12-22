"use strict"

var NativeExtension = require('../../../')
var debug = require('debug')('node')
const EventEmitter = require('events')

var Torrent = require('./torrent')

const _processDhtGetPeersReplyAlert = Symbol('processDhtGetPeersReplyAlert')
const _listenSucceededAlert = Symbol('listenSucceededAlert')
const _metadataReceivedAlert = Symbol('metadataReceivedAlert')
const _metadataFailedAlert = Symbol('metadataFailedAlert')
const _addTorrentAlert = Symbol('addTorrentAlert')
const _torrentFinishedAlert = Symbol('torrentFinishedAlert')
const _stateUpdateAlert = Symbol('stateUpdateAlert')
const _torrentRemovedAlert = Symbol('torrentRemovedAlert')
const _torrentResumedAlert = Symbol('torrentResumedAlert')
const _saveResumeDataAlert = Symbol('saveResumeDataAlert')
const _saveResumeDataFailedAlert = Symbol('saveResumeDataFailedAlert')
const _torrentPausedAlert = Symbol('torrentPausedAlert')
const _torrentCheckedAlert = Symbol('torrentCheckedAlert')
const _peerConnectAlert = Symbol('peerConnectAlert')
const _peerDisconnectedAlert = Symbol('peerDisconnectedAlert')
const _readPieceAlert = Symbol('readPieceAlert')
const _pieceFinishedAlert = Symbol('pieceFinishedAlert')
const _pluginStatus = Symbol('pluginStatus')
const _requestResult = Symbol('requestResult')

class Node extends EventEmitter {

    constructor () {
      super()
      this.session = new NativeExtension.SessionWrap()
      this.plugin = null
      this.torrents = new Map()

      // Pop alerts every seconde
      setInterval(function () {
        var alerts = this.session.popAlerts()
        for (var i in alerts) {
          this.process(alerts[i])
        };
      }.bind(this), 1000)
    }

    pause () {
      this.session.pause()
    }

    isPaused () {
      return this.session.isPaused()
    }

    resume () {
      this.session.resume()
    }

    listenPort () {
      return this.session.listenPort()
    }

    addTorrent (uploadLimit, downloadLimit, name, resumeData, savePath, infoHash, callback) {
      this.session.addTorrent(uploadLimit, downloadLimit, name, resumeData, savePath, infoHash, callback)
    }

    process (alert) {
      switch (alert.type()) {

        // dht_get_peers_reply_alert
        case 87:
          this[_processDhtGetPeersReplyAlert](alert)
          break

        // listen_succeeded_alert
        case 49:
          this[_listenSucceededAlert](alert)
          break

        // metadata_received_alert
        case 45:
          this[_metadataReceivedAlert](alert)
          break

        // metadata_failed_alert
        case 44:
          this[_metadataFailedAlert](alert)
          break

        // add_torrent_alert
        case 67:
          this[_addTorrentAlert](alert)
          break

        // torrent_finished_alert
        case 26:
          this[_torrentFinishedAlert](alert)
          break

        // state_update_alert
        case 68:
          this[_stateUpdateAlert](alert)
          break

        // torrent_removed_alert
        case 4:
          this[_torrentRemovedAlert](alert)
          break

        // torrent_resumed_alert
        case 40:
          this[_torrentResumedAlert](alert)
          break

        // save_resume_data_alert
        case 37:
          this[_saveResumeDataAlert](alert)
          break

        // save_resume_data_failed_alert
        case 38:
          this[_saveResumeDataFailedAlert](alert)
          break

        // torrent_paused_alert
        case 39:
          this[_torrentPausedAlert](alert)
          break

        // torrent_checked_alert
        case 41:
          this[_torrentCheckedAlert](alert)
          break

        // peer_connect_alert
        case 23:
          this[_peerConnectAlert](alert)
          break

        // peer_disconnected_alert
        case 24:
          this[_peerDisconnectedAlert](alert)
          break

        // read_piece_alert
        case 5:
          this[_readPieceAlert](alert)
          break

        // torrent_resumed_alert
        case 27:
          this[_pieceFinishedAlert](alert)
          break

        // PluginStatus
        case 10001:
          this[_pluginStatus](alert)
          break

        // RequestResult
        case 10002:
          this[_requestResult](alert)
          break

        default:
          debug('Alert ' + alert.what() + ' ignored')
          break
      }
    }

    // private
    [_processDhtGetPeersReplyAlert](alert) {
      // Logic here
    }

    [_listenSucceededAlert](alert) {
      // Need to send endpoint...
      this.emit('listenSucceededAlert')
    }

    [_metadataReceivedAlert](alert) {
      var h = alert.handle()

    }

    [_metadataFailedAlert](alert) {
      // Logic here
    }

    [_addTorrentAlert](alert) {
      if (!alert.error()) {
        var torrentHandle = alert.handle()
        var resumeData = alert.params().resumeData()

        var torrent = this.torrents.get(torrentHandle.infoHash())
        // Verify if torrent not already in torrents list
        if (!torrent) {

          var torrent = new Torrent(torrentHandle,
                                    resumeData,
                                    this.plugin)

          // Add torrent to torrents map
          this.torrents.set(torrentHandle.infoHash(),torrent)
          // Emit event 'addTorrentAlert'
          this.emit('addTorrentAlert')
        } else {
          torrent.resumeData = resumeData
        }

      }
    }

    [_torrentFinishedAlert](alert) {
      // Logic here
    }

    [_stateUpdateAlert](alert) {
      // Logic here
    }

    [_torrentRemovedAlert](alert) {
      // Logic here
    }

    [_torrentResumedAlert](alert) {
      // Logic here
    }

    [_saveResumeDataAlert](alert) {
      // Logic here
    }

    [_saveResumeDataFailedAlert](alert) {
      // Logic here
    }

    [_torrentPausedAlert](alert) {
      // Logic here
    }

    [_torrentCheckedAlert](alert) {
      // Logic here
    }

    [_peerConnectAlert](alert) {
      // Logic here
    }

    [_peerDisconnectedAlert](alert) {
      // Logic here
    }

    [_readPieceAlert](alert) {
      // Logic here
    }

    [_pieceFinishedAlert](alert) {
      // Logic here
    }

    [_pluginStatus](alert) {
      // Logic here
    }

    [_requestResult](alert) {
      // Logic here
    }

}

module.exports = Node
