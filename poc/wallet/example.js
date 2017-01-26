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
  console.log("Unhandled Error - Exiting")
  console.log(e)
  process.exit()
}

let buyerWallet = new SPVWallet({
      prefix  : '/Users/mokhtar/test-walleta/',
      network : 'testnet',
      logger  : logger,
      httpPort: 18332
    })

let sellerWallet = new SPVWallet({
      prefix  : '/Users/mokhtar/test-walletb/',
      network : 'testnet',
      logger  : logger,
      httpPort: 18331
    })

let payorSk = bcoin.ec.generatePrivateKey()
let payorPublicKey = bcoin.ec.publicKeyCreate(payorSk)
let payeeSk = bcoin.ec.generatePrivateKey()
let payeePublicKey = bcoin.ec.publicKeyCreate(payeeSk)


co.wrap(function*(buyer, seller){

  // we need to handle errors on initial connection failure, DNS lookup
  // what if we loose network connection.. after successful connection?

  // we can open while offline
  yield buyer.start()
  yield seller.start()

  //yield buyer.connect() // throws if we don't have connectivity
  //yield seller.connect()
  //buyer.startSync()
  //seller.startSync()

  let rawoutput = joystream.commitmentToOutput({
    value: 25000,
    payor: payorPublicKey,
    payee: payeePublicKey,
    locktime: 1
  })

  let output = bcoin.output.fromRaw(rawoutput)

  // as the buyer the transaction will go straight into the wallet database
  let contract = yield buyer.send([output])

  // getting here doesn't gurantee that the contract was broadcast successfully
  // on the network because the node catches errors from the broadcast method
  // but it does mean the contract was constructed and is stored in the wallet

  // so we need to handle possibility that contract is not in the network

  console.log('sent contract txid:', contract.txid())

  try {
    yield buyer.awaitTransaction(contract.hash(), 20000)
    console.log('buyer saw contract')
  } catch(e) {
    console.log('buyer timeout waiting to see contract')
  }

  // sleep - simulate network delay while buyer sends seller
  // contract info
  yield new Promise(function(resolve){
    setTimeout(resolve, 5000)
  })

  console.log('seller waiting to see transaction')

  let tx = yield seller.awaitTransaction(contract.hash())

  console.log('seller saw contract', tx)

})(buyerWallet, sellerWallet).catch(logExit)
