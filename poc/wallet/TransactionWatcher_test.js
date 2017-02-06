const Watcher = require('./TransactionWatcher')
const assert = require('assert')

function PoolMock() {

}

PoolMock.prototype.setFilter = function setFilter(filter){
  this.spvFilter = filter
}

PoolMock.prototype.watch = function watch(txid) {
  console.log('watch() called:', txid)
}

let pool = new PoolMock();

PoolMock.prototype.on = function on(ev, callback){
   if(ev == "tx") {
       this.callback = callback;
       console.log('set callback')
   }
}

let watcher = new Watcher(pool, true);

let p = watcher.watch("txid1");

// test interception
pool.setFilter({
  values: [],
  add: function(x){
    console.log('add on filter called')
    this.values.push(x)
  }
})

assert(pool.spvFilter.values[0] == 'txid1')

// test watching
pool.callback({
   data: "I'm a Tx",
   hash: function(){
     console.log('hash() called')
	   return "txid1";
   }
});

p.then(function(tx){
  console.log('resolved')
	console.log(tx);
}).catch(function(err){
  console.log('rejected')
  console.log(err)
})
