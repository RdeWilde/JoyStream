"use strict";

const bcoin = require('bcoin')
const _ = require('lodash')
const co =require('co')
const SPVWallet = require('./SPVWallet')
const joystream = require('joystream')

let logger = new bcoin.logger({
    level: 'info',
    color: true
});

function logExit(e) {
  console.log(e)
  process.exit()
}

// get two nodes working (fix port conflict)

let walleta = new SPVWallet({
      prefix  : '/Users/mokhtar/test-walleta/',
      network : 'testnet',
      logger  : logger,
    })

// let walletb = new SPVWallet({
//       prefix  : '/Users/mokhtar/test-walletb/',
//       network : 'testnet',
//       logger  : logger,
//     })

let payorSk = bcoin.ec.generatePrivateKey()
let payorPublicKey = bcoin.ec.publicKeyCreate(payorSk)
let payeeSk = bcoin.ec.generatePrivateKey()
let payeePublicKey = bcoin.ec.publicKeyCreate(payeeSk)


co.wrap(function*(wallet){

  yield wallet.start()

  let balance = yield wallet.getBalance();

  console.log(balance);

  let addresses = yield wallet.getAllAddresses();

  _.each(addresses, function(addr){
    console.log(addr);
  })

  let rawoutput = joystream.commitmentToOutput({
    value: 30000,
    payor: payorPublicKey,
    payee: payeePublicKey,
    locktime: 1
  })

  let output = bcoin.output.fromRaw(rawoutput)

  // as the buyer the transaction will go straight into the wallet database
  let contract = yield wallet.send([output])

  console.log('sent contract:', contract.txid())

  // sleep for 2s - simulate network delay while buyer sends seller
  // contract info
  yield new Promise(function(resolve){
    setTimeout(resolve, 2000)
  })

  // pretend we are seller .. wait to see contract
  let tx = yield wallet.awaitTransaction(contract.hash())

  console.log('saw contract', tx)

  // asssert failure on disconnect !?
  //yield wallet.disconnect()

  throw new Error('Done')
})(walleta).catch(logExit)
