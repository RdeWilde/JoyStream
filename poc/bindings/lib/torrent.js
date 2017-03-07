'use strict'

var sha1 = require('sha1')
const EventEmitter = require('events')
var Peer = require('./peer')
var TorrentPlugin = require('./torrentPlugin')
const StateT = require('./StateT')
var debug = require('debug')('torrent')
var _ = require('lodash')


class Torrent extends EventEmitter {

  constructor (handle, resumeData, plugin) {
    super()
    this.handle = handle
    this.resumeData = resumeData
    this.plugin = plugin
    this.torrentPlugin = null
    this.peers = new Map()
    this.announcedJSPeersAtTimestamp = new Map()
  }

  // Review needed !
  secondaryInfoHash () {
    var newInfoHash = this.handle.infoHash() + '_JS'
    return sha1(newInfoHash)
  }

  addJSPeerAtTimestamp (address, timestamp) {
    this.announcedJSPeersAtTimestamp.set(address, timestamp)
  }

  addPeer(ip, peerPlugin = null) {
    var peersInfo = this.handle.getPeerInfo()

    if (!this.peers.has(ip.address + ':' + ip.key)) {
      for (var i in peersInfo) {
        // Should happen only once
        if (_.isEqual(peersInfo[i].ip, ip)) {
          var peer = new Peer(ip, peerPlugin)
          this.peers.set(ip.address + ':' + ip.key, peer)

          this.emit('peerAdded', peer)
        }
      }
    } else {
      debug('Peer already added')
    }

  }

  removePeer(ip) {
    if (this.peers.has(ip.address + ':' + ip.key)) {
      this.peers.delete(ip.address + ':' + ip.key)
      this.emit('peerRemoved', ip)
    } else {
      debug('Not in peers list')
    }
  }

  setResumeDataGenerationResult (resumeData) {
    this.resumeData = resumeData
    this.emit('resume_data_generation_completed', resumeData)
  }

  addTorrentPlugin (torrentPluginStatus) {
    if (this.torrentPlugin) {
      debug('This torrent already have a torrentPlugin')
    } else {
      this.torrentPlugin = new TorrentPlugin(torrentPluginStatus, this.plugin)
      this.emit('torrentPluginAdded')
    }
  }

  removeTorrentPlugin () {
    if (this.torrentPlugin) {
      this.torrentPlugin = null
      this.emit('torrentPluginRemoved')
    } else {
      debug('Cannot remove torrentPlugin because undefined')
    }
  }

  toSellMode (sellerTerms, callback) {
    var infoHash = this.handle.infoHash()

    // Verify if torrentPlugin set
    if (this.torrentPlugin) {
      // Verify torrent state
      if (this.handle.status().state === StateT.SEEDING) {
        this.plugin.to_sell_mode(infoHash, sellerTerms, (err, result) => {
          if (!err) {
            this.plugin.start(infoHash, (err, result) => {
              if (!err) {
                debug('Plugin started')

                // Verify first if we have already a connection
                this.peers.forEach((peer)=> {
                  if (peer.status)
                    console.log(peer.status)
                })

                // Regularly request status alerts from peer plugins on this torrent
                setInterval(() => {
                  this.plugin.post_peer_plugin_status_updates(infoHash)
                }, 1000)

                this.on('newConnection', (connection) => {
                  console.log(connection)
                })

              }
              callback(err, result)
            })
          } else {
            callback(err, result)
          }
        })
      } else {
        debug('Torrent not in seeding state')
        callback(new Error('Torrent not in seeding state'), null)
      }
    } else {
      debug('TorrentPlugin not set for this torrent')
      callback(new Error('TorrentPlugin not set for this torrent'), null)
    }
  }

  toBuyMode (buyerTerms, callback) {
    var infoHash = this.handle.infoHash()

    if (this.torrentPlugin) {
      if (this.handle.status().state === StateT.DOWNLOADING) {
        /*
          TODO:
          - verify fund in wallet
          - Lock minimum funds
          - Get TorrentPlugin
          - Need paychanKeysGenerator, receiveAddressesGenerator, changeAddressesGenerator ?
        */

        this.plugin.to_buy_mode(infoHash, buyerTerms, (err, result) => {
          debug('IN BUYING MODE !')
          if (!err) {
            this.plugin.start(infoHash, callback)
          } else {
            callback(err, result)
          }
        })
      } else {
        debug('Torrent not in downloading state')
        callback(new Error('Torrent not in downloading state'), null)
      }
    } else {
      debug('TorrentPlugin not set for this torrent')
      callback(new Error('TorrentPlugin not set for this torrent'), null)
    }
  }

  observeTorrent (callback) {
    // var infoHash = this.handle.infoHash()
    /*
      TODO
    */
  }
}

module.exports = Torrent
