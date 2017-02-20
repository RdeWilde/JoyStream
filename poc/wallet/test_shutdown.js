const co = require('co')

const bcoin = require('bcoin')

let node = new bcoin.spvnode({
      db: 'leveldb',
      prefix  : '/Users/mokhtar/test-walleta/',
      network : 'testnet',
      witness: false,
      logger: new bcoin.logger({
        level: 'debug',
        color: true
      })
})

node.on('error', function(){})



let sleep = co.wrap(function*sleep(timeout){
  return new Promise(function(resolve){
    setTimeout(resolve, timeout)
  })
})

// co(function* main(){
//   yield node.open()
//   yield node.connect()
//   console.log('connected')
//   yield sleep(5000)
//   console.log('disconnecting...')
//   yield node.disconnect()
//   yield sleep(5000)
//   console.log('reconnecting...')
//   yield node.connect()
// })

co(function* main(){
  console.log('NETWORK', node.network)
  yield node.open()
  yield node.connect()
  console.log('connected')
  yield sleep(5000)
  console.log('closing...')
  yield node.close()
  console.log('NETWORK', node.network)

  yield sleep(5000)
  console.log('opening...')
  yield node.open()
  console.log('conecting')
  yield node.connect()
  node.startSync()
})
