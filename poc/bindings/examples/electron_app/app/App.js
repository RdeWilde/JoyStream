import React, { Component } from 'react'
import { Node } from '../../../'

var node = new Node()

class App extends Component {

  constructor (props) {
    super(props)

    this.addTorrent = this.addTorrent.bind(this)
    this.addTorrentFile = this.addTorrentFile.bind(this)
    this.torrentAdded = this.torrentAdded.bind(this)
    this.buyTorrent = this.buyTorrent.bind(this)

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
      '', null,
      '/home/lola/joystream/test/',
      'd59e6da0de8f5382f067e07375c262f15570a8f1',
      this.torrentAdded)

    // Buy
  }

  addTorrent () {
    node.addTorrent(0, 0,
      '306497171', null,
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
      var status = torrentHandle.status()

      if (!torrentInfo) {
        // torrent_info not yet set need to come from peers
        torrent.on('metadata_received_alert', (torrentInfo) => {
          this.forceUpdate()
        })
      } else {

        torrent.on('state_update_alert', (state, progress) => {
          this.forceUpdate()
        })

        torrent.on('torrent_finished_alert', () =>{
          this.forceUpdate()
        })

        var statusText;

        switch(status.state) {
          case 0:
            statusText = 'Queued for checking'
            break
          case 1:
            statusText = 'Checking files'
            break
          case 2:
            statusText = 'Downloading metadata'
            break
          case 3:
            statusText = 'Downloading'
            break
          case 4:
            statusText = 'Finished'
            break
          case 5:
            statusText = 'Seeding'
            break
          case 6:
            statusText = 'Allocating'
            break
          case 6:
            statusText = 'Checking resume data'
            break
          default:
            statusText = 'Unknown'
        }

        rows.push(
          <tr>
            <td>{torrentInfo.name()}</td>
            <td>{Number(torrentInfo.totalSize() / 1000000).toFixed(2)} Mb</td>
            <td>{Number(status.progress*100).toFixed(0)}%</td>
            <td>{statusText}</td>
          </tr>)
      }
    })

    return (
      <div className="container">
        <h1>Joystream</h1>
        <br/>
        <a href="#" onClick={this.addTorrent} > Add a torrent </a>
        <br/>
        <a href="#" onClick={this.buyTorrent} > Buy a torrent </a>
        <br/>
        <a href="#" onClick={this.addTorrentFile} > Add a torrent with torrent file </a>
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
