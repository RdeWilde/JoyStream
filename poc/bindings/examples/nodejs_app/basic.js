var nativeExtension = require('../../');

var session = new nativeExtension.SessionWrap();

setInterval(function(){
  process.stdout.write(".");
}, 2000);

session.pause();
