# joystream-core

The core library for a JoyStream node.

## Documentation

...coming later...

### Handle Hierarchy
There is a hierarchy of handles corresponding to internal structure of a node instance.

- Node
    - (*) Torrent
	    - (*) Peer
	    - (1) TorrentPlugin
	        - (*) PeerPlugin
			- (1) Session
				- (0|1) Buying
				- (0|1) Selling
				- (*) Connection
					- (1) CBStateMachine
						- (1) Payor
						- (1) Payee

### Handle Expiry
For each handle, monitor the given kill signal to detect expiry.

Type | Expiry Signal
------------ | -------------
Node 			| none
Torrent 		| Node::torrentRemoved
Peer 			| Torrent::peerRemoved
TorrentPlugin 		| Torrent::torrentPluginRemoved
PeerPlugin		| Torrent::peerPluginRemoved
Session 		| Node::torrentPluginRemoved
Buying			| Node::modeChanged
Selling			| Node::modeChanged
Connection		| Session::connectionRemoved
CBStateMachine		| same
Payor			| same
Payee 			| same

## Building

```
qmake core.pro
make
```

## Testing

Build and run ../test/core
