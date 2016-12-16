var nativeExtension = require('../../');

var session = new nativeExtension.SessionWrap();

setInterval(function(){
  process.stdout.write(".");
}, 2000);

session.pause();

console.log(session.isPaused());

session.resume();

console.log(session.isPaused());

console.log(session.listenPort());

session.addTorrent(0,0,'Sintel',null,'/home/lola/joystream/test/','6a9759bffd5c0af65319979fb7832189f4f3c35d',
  function(err, smth) {
    console.log('This has been called once the torrent is added')
});
