'use strict'

var NativeExtension = require('bindings')('NativeExtension')
var debug = require('debug')('node')
const EventEmitter = require('events')
const Torrent = require('./torrent')
var _ = require('lodash')

const _processDhtGetPeersReplyAlert = Symbol('processDhtGetPeersReplyAlert')
const _listenSucceededAlert = Symbol('listenSucceededAlert')
const _metadataReceivedAlert = Symbol('metadataReceivedAlert')
const _metadataFailedAlert = Symbol('metadataFailedAlert')
const _addTorrentAlert = Symbol('addTorrentAlert')
const _torrentFinishedAlert = Symbol('torrentFinishedAlert')
const _stateUpdateAlert = Symbol('stateUpdateAlert')
const _stateChangedAlert = Symbol('stateChangedAlert')
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
const _requestResult = Symbol('requestResult')
const _torrentPluginStatusUpdateAlert = Symbol('torrentPluginStatusUpdateAlert')
const _peerPluginStatusUpdateAlert = Symbol('peerPluginStatusUpdateAlert')
const _torrentPluginAdded = Symbol('torrentPluginAdded')
const _torrentPluginRemoved = Symbol('torrentPluginRemoved')
const _peerPluginAdded = Symbol('peerPluginAdded')
const _peerPluginRemoved = Symbol('peerPluginRemoved')
const _connectionAddedToSession = Symbol('connectionAddedToSession')
const _connectionRemovedFromSession = Symbol('connectionRemovedFromSession')
const _sessionStarted = Symbol('sessionStarted')
const _sessionStopped = Symbol('sessionStopped')
const _sessionPaused = Symbol('sessionPaused')
const _sessionToObserveMode = Symbol('sessionToObserveMode')
const _sessionToSellMode = Symbol('sessionToSellMode')
const _sessionToBuyMode = Symbol('sessionToBuyMode')
const _validPaymentReceived = Symbol('validPaymentReceived')
const _invalidPaymentReceived = Symbol('invalidPaymentReceived')
const _lastPaymentReceived = Symbol('lastPaymentReceived')
const _sellerTermsUpdated = Symbol('sellerTermsUpdated')
const _sentPayment = Symbol('sentPayment')
const _contractConstructed = Symbol('contractConstructed')
const _validPieceArrived = Symbol('validPieceArrived')
const _invalidPieceArrived = Symbol('invalidPieceArrived')
const _buyerTermsUpdated = Symbol('buyerTermsUpdated')
const _anchorAnnounced = Symbol('anchorAnnounced')


const minimumMessageId = 60

/*
 * Class Node
 * Manage the alerts and execute the differents request (add_torrent, buy_torrent,...)
 * TODO: File too big. The alerts process should go in another class.
 */

class Node extends EventEmitter {

    constructor (port) {
      super()
      this.session = new NativeExtension.Session(port)
      this.plugin = new NativeExtension.Plugin(minimumMessageId)
      this.torrents = new Map()
      this.torrentsBySecondaryHash = new Map()

      // Add plugin to session
      this.session.addExtension(this.plugin)

      // Pop alerts every seconde
      setInterval(function () {
        var alerts = this.session.popAlerts()
        for (var i in alerts) {
          this.process(alerts[i])
        }
      }.bind(this), 1000)
    }

    addTorrent (addTorrentParams, callback) {
        this.plugin.add_torrent(addTorrentParams, (err, torrentHandle) => {

            if (!err) {
            var torrent = this.torrents.get(torrentHandle.infoHash())
            // Verify if torrent not already in torrents list
            if (!torrent) {

                var torrent = new Torrent(torrentHandle,
                    null,
                    this.plugin)
                // Add torrent to torrents map
                this.torrents.set(torrentHandle.infoHash(),torrent)
                // Emit event 'addTorrentAlert'
                this.emit('add_torrent_alert', torrent)

                // DHT stuff
                this.torrentsBySecondaryHash.set(torrent.secondaryInfoHash(), torrentHandle.infoHash())
                this.session.dhtAnnounce(torrent.secondaryInfoHash(), this.listenPort())

            } else {
                torrent.resumeData = resumeData
            }
            debug('Adding torrent succeeded.')
            callback(err, torrent)
        } else {
            // Need error wrapper for message
            debug('Adding torrent failed')
            callback(err, null)
        }
    })
    }

    removeTorrent (infoHash, callback) {
        var torrent = this.torrents.get(infoHash)

        if (torrent) {
            this.plugin.remove_torrent(infoHash, (err, result) => {
                if (!err)
            this.torrents.delete(infoHash)
        })
        } else {
            debug('Cannot remove torrent : Torrent not found')
            callback(new Error('Cannot remove torrent : Torrent not found'), null)
        }

    }

    /**********************
     *
     *  Session methods
     *
     *********************/
    pause (callback) {
      this.session.pause(callback)
    }

    isPaused () {
      return this.session.isPaused()
    }

    listenPort () {
      return this.session.listenPort()
    }

    /**********************
     *
     *  Alerts
     *
     *********************/
    process (alert) {
      debug('Alert ' + alert.what + ' received !')

      switch (alert.type) {

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

        // state_changed_alert
        case 10:
          this[_stateChangedAlert](alert)
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

        // TorrentPluginStatusUpdateAlert
        case 10001:
          this[_torrentPluginStatusUpdateAlert](alert)
          break

        // PeerPluginStatusUpdateAlert
        case 10002:
          this[_peerPluginStatusUpdateAlert](alert)
          break

        // RequestResult
        case 10003:
          this[_requestResult](alert)
          break

        // TorrentPluginAdded
        case 10005:
          this[_torrentPluginAdded](alert)
          break

        // TorrentPluginRemoved
        case 10006:
          this[_torrentPluginRemoved](alert)
          break

        // PeerPluginAdded
        case 10008:
          this[_peerPluginAdded](alert)
          break

        // PeerPluginRemoved
        case 10009:
          this[_peerPluginRemoved](alert)
          break

        // ConnectionAddedToSession
        case 100011:
          this[_connectionAddedToSession](alert)
          break

          // ConnectionRemovedFromSession
        case 100012:
          this[_connectionRemovedFromSession](alert)
          break

        // SessionStarted
        case 100013:
          this[_sessionStarted](alert)
          break

        // SessionStopped
        case 100014:
          this[_sessionStopped](alert)
          break

        // SessionPaused
        case 100015:
          this[_sessionPaused](alert)
          break

        // SessionToObserveMode
        case 100016:
          this[_sessionToObserveMode](alert)
          break

        // SessionToSellMode
        case 100017:
          this[_sessionToSellMode](alert)
          break

        // SessionToBuyMode
        case 100018:
          this[_sessionToBuyMode](alert)
          break

        // ValidPaymentReceived
        case 100019:
          this[_validPaymentReceived](alert)
          break

        // InvalidPaymentReceived
        case 100020:
          this[_invalidPaymentReceived](alert)
          break

        // LastPaymentReceived
        case 100023:
          this[_lastPaymentReceived](alert)
          break

        // SellerTermsUpdated
        case 100024:
          this[_sellerTermsUpdated](alert)
          break

        // SentPayment
        case 100025:
          this[_sentPayment](alert)
          break

        // ContractConstructed
        case 100026:
          this[_contractConstructed](alert)
          break

        // ValidPieceArrived
        case 100027:
          this[_validPieceArrived](alert)
          break

        // InvalidPieceArrived
        case 100028:
          this[_invalidPieceArrived](alert)
          break

        // BuyerTermsUpdated
        case 100029:
          this[_buyerTermsUpdated](alert)
          break

        // AnchorAnnounced
        case 10004:
          this[_anchorAnnounced](alert)
          break

        default:
          debug('Alert ' + alert.what + ' ignored')
          break
      }
    }

    /*
     * Private Method
     */

    [_processDhtGetPeersReplyAlert](alert) {
      var torrentSecondaryHash = this.torrentsBySecondaryHash.get(alert.infoHash)
      var torrent = this.torrents.get(torrentSecondaryHash)
      if (torrent) {
        var timestamp = Date.now()
        var peers = alert.peers
        for (var i in peers) {
          torrent.addJSPeerAtTimestamp(peers[i].address, timestamp)
          torrent.handle.connectPeer(peers[i])
          debug('Connection added')
        }
      } else {
        debug('Torrent with secondaryInfoHash ' + alert.infoHash + ' not found')
      }
    }

    [_listenSucceededAlert](alert) {
      var endpoint = alert.endpoint
      this.emit('listen_succeeded_alert', endpoint)
    }

    [_metadataReceivedAlert](alert) {
      var torrentHandle = alert.handle
      var torrentInfo = torrentHandle.torrentFile()
      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrentInfo && torrent) {
        debug('Received Metadata for torrent')
        torrent.emit('metadata_received_alert', torrentInfo)
      }

    }

    [_metadataFailedAlert](alert) {
      // what to do?
    }

    [_addTorrentAlert](alert) {
      if (!alert.error) {
        var torrentHandle = alert.handle
        var resumeData = alert.addTorrentParams.resumeData

        var torrent = this.torrents.get(torrentHandle.infoHash())
        // Verify if torrent not already in torrents list
        if (!torrent) {

          var torrent = new Torrent(torrentHandle,
                                    resumeData,
                                    this.plugin)

          // Add torrent to torrents map
          this.torrents.set(torrentHandle.infoHash(),torrent)

          // DHT stuff
          this.torrentsBySecondaryHash.set(torrent.secondaryInfoHash(), torrentHandle.infoHash())
          this.session.dhtAnnounce(torrent.secondaryInfoHash(), this.session.listenPort)

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
      var torrentHandle = alert.handle

      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        torrent.emit('torrent_finished_alert')
      } else {
        debug('Torrent not found')
      }

    }

    [_stateUpdateAlert](alert) {
      var status = alert.status

      for (var i in status) {
        var torrent = this.torrents.get(status[i].infoHash)

        if (torrent) {
          torrent.emit('state_update_alert', status[i].state, status[i].progress)
        } else {
          debug('Torrent not found !')
        }
      }
    }

    [_stateChangedAlert](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        torrent.emit('state_changed_alert')
      } else {
        debug('Torrent not found')
      }

    }

    [_torrentRemovedAlert](alert) {
      /*
       * NOTICE: Docs say p->handle may be invalid at this time - likely because this is a removal operation,
       * so we must use p->info_hash instead.
       */
       var torrent = this.torrents.get(alert.infoHash)

       if (torrent) {
         torrent.emit('torrent_removed_alert')
       } else {
         debug('Torrent not found')
       }
    }

    [_torrentResumedAlert](alert) {
      var infoHash = alert.handle.infoHash()
      var torrent = this.torrents.get(infoHash)

      if (torrent) {
        // emit alert that the torrent has been resumed
        debug('Torrent resumed')
        torrent.emit('torrent_resumed_alert')
      } else {
        var err = 'Cannot find torrent to resume'
        debug(err)
        throw new Error(err)
      }
    }

    [_saveResumeDataAlert](alert) {
      var torrentHandle = alert.handle

      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        var resumeData = NativeExtension.BEncode(alert.resumeData)
        torrent.setResumeDataGenerationResult(resumeData)
      } else {
        debug('Torrent not found !')
      }

    }

    [_saveResumeDataFailedAlert](alert) {
      var torrentHandle = alert.handle

      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        // Save empty resume data in torrent, is required
        // to trigger event signal
        var resumeData = ''
        torrent.setResumeDataGenerationResult(resumeData)
      } else {
        debug('Torrent not found !')
      }
    }

    [_torrentPausedAlert](alert) {
      var infoHash = alert.handle.infoHash()
      var torrent = this.torrents.get(infoHash)

      /* Need to verify if is_all_zero() ?
       * If all_zero torrent not find so probably not
       */

      if (torrent) {
        // emit alert that the torrent has been paused
        debug('Torrent paused')
        torrent.emit('torrent_paused_alert')
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
      var torrentHandle = alert.handle
      var peersInfo = torrentHandle.getPeerInfo()

      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        for (var i in peersInfo) {
          if (_.isEqual(peersInfo[i].ip, alert.ip)) {
            torrent.addPeer(peersInfo[i])
          }
        }
      } else {
        debug('Torrent not found')
      }

    }

    [_peerDisconnectedAlert](alert) {
      var torrentHandle = alert.handle

      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        torrent.removePeer(alert.ip)
      } else {
        debug('Torrent not found')
      }
    }

    [_readPieceAlert](alert) {
      // Nothing todo here ?
    }

    [_pieceFinishedAlert](alert) {
      // Nothing to do here ?
    }

    [_requestResult](alert) {
      /** IF A CALLBACK DID NOT CATCH ALL EXCEPTIONS,
       * THEN RESULT IS THROWN OUT HERE **/
      alert.run();
    }

    [_torrentPluginStatusUpdateAlert](alert) {
      var statuses = alert.statuses

      for (var [infoHash, torrentPluginStatus] of statuses) {
        var torrent = this.torrents.get(infoHash)
        torrent.torrentPlugin.update(torrentPluginStatus)
      }
      this.emit('TorrentPluginStatusUpdateAlert', statuses)
    }

    [_peerPluginStatusUpdateAlert](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())
      var statuses = alert.statuses

      if (!torrent.plugin) {
        debug('No plugin find')
      } else {
        torrent.plugin.emit('updatePeerPluginStatuses', statuses)
        for (var [endpoint, peerPluginStatus] of statuses) {
          var peer = torrent.peers.get(endpoint)
          if (peer) {
            peer.plugin.update(peerPluginStatus)
          } else {
            debug('Peer not found !')
          }
        }
      }
    }

    [_torrentPluginAdded](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        debug('Torrent already created')
      } else {
        torrent = new Torrent(torrentHandle, '', this.plugin)
        this.torrents.set(torrentHandle.infoHash(), torrent)
      }
      torrent.addTorrentPlugin(alert.torrentPluginStatus)
    }

    [_torrentPluginRemoved](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      if (torrent) {
        if (torrent.torrentPlugin) {
          torrent.removeTorrentPlugin()
        }
      } else {
        debug('Torrent not found')
      }
    }

    [_peerPluginAdded](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())
      var peer = torrent.peers.get(alert.ip)

      if (peer) {
        if (!peer.peerPlugin) {
          peer.addPeerPlugin(alert.status)
        } else {
          debug('PeerPlugin already initialized')
        }
      } else {
        debug('Peer not found !')
      }

    }

    [_peerPluginRemoved](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())
      var peer = torrent.peers.get(alert.ip)

      if (peer) {
        if (!peer.peerPlugin) {
          peer.removePeerPlugin()
        } else {
          debug('PeerPlugin already initialized')
        }
      } else {
        debug('Peer not found !')
      }
    }

    [_connectionAddedToSession](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())
      var peerPlugin = torrent.peers.get(alert.ip).plugin

      peerPlugin.emit('connectionAdded', alert.connectionStatus)
    }

    [_connectionRemovedFromSession](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())
      var peer = torrent.peers.get(alert.ip)

      peer.plugin.emit('connectionRemoved')
    }

    [_sessionStarted](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sessionStarted')
    }

    [_sessionPaused](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sessionPause')
    }

    [_sessionStopped](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sessionStopped')
    }

    [_sessionToObserveMode](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sessionToObserveMode')
    }

    [_sessionToSellMode](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('SessionToSellMode', alert)
    }

    [_sessionToBuyMode](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('SessionToBuyMode', alert)
    }

    [_validPaymentReceived](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('validPaymentReceived', alert)
    }

    [_invalidPaymentReceived](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('invalidPaymentReceived', alert)
    }

    [_buyerTermsUpdated](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('buyerTermsUpdated', alert)
    }

    [_sellerTermsUpdated](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sellerTermsUpdated', alert)
    }

    [_contractConstructed](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('contractConstructed', alert)
    }

    [_sentPayment](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('sentPayment', alert)
    }

    [_lastPaymentReceived](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('lastPaymentReceived', alert)
    }

    [_invalidPieceArrived](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('invalidPieceArrived', alert)
    }

    [_validPieceArrived](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('validPieceArrived', alert)
    }

    [_anchorAnnounced](alert) {
      var torrentHandle = alert.handle
      var torrent = this.torrents.get(torrentHandle.infoHash())

      torrent.plugin.emit('AnchorAnnounced', alert)
    }
}

module.exports = Node
