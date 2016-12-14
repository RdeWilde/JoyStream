var nativeExtension = require('../../');
var obj = new nativeExtension.MyObject(0);

setInterval(function(){
  process.stdout.write(".");
}, 2000);
