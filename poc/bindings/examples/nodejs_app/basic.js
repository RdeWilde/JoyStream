var nativeExtension = require('../../');
var obj = new nativeExtension.MyObject(0);

var http = require('http');

var server = http.createServer(function(request, response) {

});

server.listen(9090);
