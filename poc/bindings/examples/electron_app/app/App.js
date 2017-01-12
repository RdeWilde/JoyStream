import React, { Component } from 'react'
import { Node } from '../../../'

var node = new Node()

class App extends Component {
  constructor () {
    super()

    this.state = {
      torrents: [],
      port: node.listenPort(),
      isPaused: node.isPaused()
    }
  }


  addTorrent () {
    node.addTorrent(0, 0, 'Sintel', null, '/home/lola/joystream/test/', '6a9759bffd5c0af65319979fb7832189f4f3c35d',
      function (err, smth) {
        if (err) {
          debug(err)
        } else {
          debug('This has been called once the torrent is added')
        }
      })
    }

  render () {
    return (
      <div className="container">
        <h1>Joystream</h1>
        <br/>
        <a href="#" onClick={this.addTorrent.bind(this)} > Add a torrent </a>
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
          </tbody>
        </table>
      </div>
    )
  }
}

export default App
