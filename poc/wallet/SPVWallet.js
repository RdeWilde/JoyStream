"user strict";

const assert = require('assert')
const _ = require('lodash')
const TransactionWatcher = require('./TransactionWatcher')
const co = require('co')

const bcoin = require('bcoin')

// set default network to be testnet
bcoin.set('testnet')

module.exports = SPVWallet;

function SPVWallet(options) {

  let self = {};

  options.prefix = options.prefix || __dirname;

  let node = new bcoin.spvnode(options);

  // add a mempool (advantage: better network fee estimation, or do we need a full chain?)
  // but actually adding it is interfering with watcher attempt to get mempool!
  //node.pool.selfish = true
  //node.pool.mempool = new bcoin.mempool({ chain: node.chain })

  // make sure to handle error event or app will exit with unhandled Error
  if(typeof options.onError === 'function') {
    node.on('error', options.onError)
  } else {
    node.on('error', function(err) {
        console.log(err.message)
    });
  }

  let watcher = new TransactionWatcher(node.pool, true)

  // start() or open() must be called before any other method on SPVWallet
  self.start = co.wrap(function* start() {
    assert(!node.wallet)

    yield node.open()

    assert(node.wallet)

    yield node.connect()

    node.startSync()

  })

  self.open = function open() {
    return node.open()
  }

  // spvwallet must be opened and pool opened to connect
  self.connect = function connect() {
    return node.connect()
  }

  self.disconnect = function disconnect() {
    return node.disconnect()
  }

  //must be connected to start syncing
  self.startSync = function startSync() {
    node.startSync()
  }

  self.stopSync = function stopSync() {
    node.stopSync()
  }

  self.getBalance = co.wrap(function* getBalance() {
    return yield node.wallet.getBalance()
  })

  self.getAddress = function getAddress() {
    return node.wallet.getAddress()
  }

  self.getAllAddresses = co.wrap(function* getAllAddresses() {
    let account = yield node.wallet.getAccount('default')

    // List all receive addresses of the default account
    let i, addresses = [];
    for(i = 0; i < account.receiveDepth; i++) {
        addresses.push(account.deriveReceive(i).getAddress());
        //pubkeyhash = account.deriveReceive(i).getAddress().getHash();
    }

    return addresses;
  })

  //transaction hash (not reversed little endian txid) hex string or Buffer
  self.awaitTransaction = function(hash, timeout) {
    return watcher.watch(hash, timeout);
  }

  // Broadcast a signed transaction (settlements, refunds)
  // Accepts hex encoded string, Buffer or bcoin.TX
  // will try upto three times on pool.options.invTimeout (1minute)
  self.broadcast = co.wrap(function* broadcast(tx) {

    if (!(tx instanceof bcoin.primitives.TX)) {
      tx = bcoin.primitives.TX.fromRaw(tx)
    }

    return yield node.broadcast(tx)
  })

  self.close = function close() {
    return node.close()
  }

  // Create, fund and sign a transaction from array of outputs
  // (commitments of a joystream contract)
  self.createAndSend = function createAndSend(outputs, feeRate) {
      // return the transaction (promise)
      return node.wallet.send({
        sort: false,
        outputs: outputs,
        rate: feeRate
      })
  }

  return self;
}

// shortcuts
SPVWallet.generatePrivateKey = function generatePrivateKey() {
  return bcoin.ec.generatePrivateKey();
}


//paychan store for buyer and tx pool for seller
