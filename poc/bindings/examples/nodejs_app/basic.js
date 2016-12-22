var Node = require('./lib/node');

var node = new Node()

node.on('listenSucceededAlert', () => {
  console.log('hey !')
})

/*var session = new nativeExtension.SessionWrap();
var torrentHandle = new nativeExtension.TorrentHandleWrap();

console.log(torrentHandle);

session.pause();

console.log(session.isPaused());

session.resume();

console.log(session.isPaused());

console.log(session.listenPort());

session.addTorrent(0,0,'Sintel',null,'/home/lola/joystream/test/','6a9759bffd5c0af65319979fb7832189f4f3c35d',
  function(err, smth) {
    console.log('This has been called once the torrent is added')
});*/
