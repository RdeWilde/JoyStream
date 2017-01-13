import React, { Component } from 'react'
import { Node } from '../../../'

var node = new Node()

class App extends Component {

  constructor (props) {
    super(props)

    this.addTorrent = this.addTorrent.bind(this)
    this.addTorrentFile = this.addTorrentFile.bind(this)
    this.torrentAdded = this.torrentAdded.bind(this)

    this.state = {
      torrents: []
    }
  }

  addTorrentFile () {
    node.addTorrent(0, 0,
      'Sintel', null,
      '/home/lola/joystream/test/',
      '6a9759bffd5c0af65319979fb7832189f4f3c35d',
      this.torrentAdded)
  }

  addTorrent () {
    node.addTorrent(0, 0,
      'Sintel', null,
      '/home/lola/joystream/test/',
      '6a9759bffd5c0af65319979fb7832189f4f3c35d',
      this.torrentAdded)
  }

  torrentAdded (err, smth) {
    if (err) {
      debug(err)
    } else {
      debug('This has been called once the torrent is added')
      this.setState({torrents: Array.from(node.torrents)})
    }
  }

  render () {
    return (
      <div className="container">
        <h1>Joystream</h1>
        <br/>
        <a href="#" onClick={this.addTorrent} > Add a torrent </a> <a href="#" onClick={this.addTorrentFile} > Add a torrent with torrent file </a>
        <br/>
        <br/>
        <table className="table">
          <thead>
            <tr>
              <th>Name</th>
              <th>Size</th>
              <th>Progress</th>
              <th>Status</th>
            </tr>
          </thead>
          <tbody>
            {this.state.torrents.map((torrent, i) => {
              // torrent = [infoHash, Torrent]
              var torrentHandle = torrent[1].handle
              var torrentInfo = torrentHandle.torrentFile()
              if (!torrentInfo) {
                // torrent_info not yet set need to come from peers
                torrent[1].on('metadata_received_alert', (torrentInfo) => {
                  console.log(torrent.info)
                })
                return (
                  <tr id={torrent[0]}>
                    <td>Nan</td>
                    <td>Nan</td>
                    <td>Nan</td>
                    <td>Nan</td>
                  </tr>)
              } else {
                return (
                  <tr>
                    <th>1</th>
                    <td>13 Mb</td>
                    <td>13%</td>
                    <td>Good</td>
                  </tr>)
              }
            })}
          </tbody>
        </table>
      </div>
    )
  }
}

export default App
