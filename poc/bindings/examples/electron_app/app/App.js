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
      torrents: new Map()
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
      this.setState({torrents: node.torrents})
    }
  }

  render () {
    let rows = [];

    this.state.torrents.forEach((torrent, infoHash) => {
      var torrentHandle = torrent.handle
      var torrentInfo = torrentHandle.torrentFile()
      if (!torrentInfo) {
        // torrent_info not yet set need to come from peers
        torrent.on('metadata_received_alert', (torrentInfo) => {
          this.forceUpdate()
        })
      } else {
        rows.push(
          <tr>
            <td>{torrentInfo.name()}</td>
            <td>{Number(torrentInfo.totalSize() / 1000000).toFixed(2)} Mb</td>
            <td>13%</td>
            <td>Good</td>
          </tr>)
      }
    })

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
            { rows }
          </tbody>
        </table>
      </div>
    )
  }
}

export default App
