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
      this.torrentsBySecondaryHash = new Map()

      // Pop alerts every seconde
      setInterval(function () {
        var alerts = []
        // To be sure all the alerts are process before getting new ones
        if (alerts.length == 0) {
          var alerts = this.session.popAlerts()
          while (alerts.length > 0) {
            // shift remve and return the first alert in the array
            var alert = alerts.shift()
            this.process(alert)
          }
        }
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

    /*
     * Private Method
     */

    [_processDhtGetPeersReplyAlert](alert) {
      var torrent = this.torrentsBySecondaryHash.get(alert.infoHash())

      if (torrent) {
        var timestamp = new Date.now()
        var peers = alert.peers()
        for (var i in peers) {
          torrent.addJSPeerAtTimestamp(peers[i], timestamp)
          torrent.handle.connectPeer(peers[i])
          debug('Connection added')
        }
      } else {
        debug('Torrent with secondaryInfoHash ' + alert.infoHash() + ' not found')
      }
    }

    [_listenSucceededAlert](alert) {
      this.emit('listenSucceededAlert', alert.endpoint())
    }

    [_metadataReceivedAlert](alert) {
      var torrentHandle = alert.handle()
      var torrentInfo = torrentHandle.torrentFile()
      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrentInfo && torrent) {
        debug('Received Metadata for torrent')
        torrent.emit('metadataReceivedAlert', torrentInfo)
      }

    }

    [_metadataFailedAlert](alert) {
      debug('Metadata Failed Alert !')
      // what to do?
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
          this.emit('addTorrentAlert', torrent)

          // DHT stuff
          this.torrentsBySecondaryHash.set(torrent.secondaryInfoHash(), torrentHandle.infoHash())
          this.session.dhtAnnounce(torrent.secondaryInfoHash(), this.session.listenPort())


        } else {
          torrent.resumeData = resumeData
        }
        debug('Adding torrent succeeded.')
      } else {
        // Need error wrapper for message
        debug('Adding torrent failed:')
      }
    }

    [_torrentFinishedAlert](alert) {
      debug('Torrent finish alert !')
      // nothing to do?
      // Maybe emit an event ?
    }

    [_stateUpdateAlert](alert) {
      var status = alert.status()

      for (var i in status) {
        var torrent = this.torrents.get(status[i].infoHash())

        if (torrent) {
          torrent.emit('stateUpdatedAlert', status[i].state(), status[i].progress())
        } else {
          debug('Torrent not found !')
        }
      }
    }

    [_torrentRemovedAlert](alert) {
      /*
       * NOTICE: Docs say p->handle may be invalid at this time - likely because this is a removal operation,
       * so we must use p->info_hash instead.
       */
       var torrent = this.torrents.get(alert.infoHash())

       if (torrent) {
         torrent.emit('torrentRemovedAlert')
       } else {
         debug('Torrent not found')
       }
    }

    [_torrentResumedAlert](alert) {
      var infoHash = alert.handle().infoHash()
      var torrent = this.torrents.get(infoHash)

      if (torrent) {
        // emit alert that the torrent has been resumed
        debug('Torrent resumed')
        torrent.emit('torrentResumedAlert')
      } else {
        var err = 'Cannot find torrent to resume'
        debug(err)
        throw new Error(err)
      }
    }

    [_saveResumeDataAlert](alert) {
      // Logic here
    }

    [_saveResumeDataFailedAlert](alert) {
      // Logic here
    }

    [_torrentPausedAlert](alert) {

      var infoHash = alert.handle().infoHash()
      var torrent = this.torrents.get(infoHash)

      /* Need to verify if is_all_zero() ?
       * If all_zero torrent not find so probably not
       */

      if (torrent) {
        // emit alert that the torrent has been paused
        debug('Torrent paused')
        torrent.emit('torrentPausedAlert')
      } else {
        var err = 'Cannot find torrent to pause'
        debug(err)
        throw new Error(err)
      }

    }

    [_torrentCheckedAlert](alert) {
       // Nothing to do ?
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
