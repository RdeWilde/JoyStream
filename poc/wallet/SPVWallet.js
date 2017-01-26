"user strict";

const assert = require('assert')
const _ = require('lodash')
const TransactionWatcher = require('./TransactionWatcher')
const co = require('co')

const bcoin = require('bcoin')

bcoin.set('testnet')

module.exports = SPVWallet;

function SPVWallet(options) {

  let self = {};

  const prefix = options.prefix || __dirname;
  const network = options.network || 'tesetnet';

  // take a bcoin spvnode instead of creating it?
  let node = new bcoin.spvnode({
    prefix : prefix,
    logger : options.logger,
    db: 'leveldb',
    maxOutbound: options.maxOutbound || 8,
    network: network,
    httpPort: options.httpPort,
  });


  // make sure to handle error event or app will exit with unhandled Error
  if(typeof options.onError === 'function') {
    node.on('error', options.onError)
  } else {
    node.on('error', function(err) {
        console.log(err.message)
    });
  }

  let watcher = new TransactionWatcher(node.pool, true)

  // open() must be called before any other method on SPVWallet
  self.start = co.wrap(function* start() {
    assert(!node.wallet)

    yield node.open()

    assert(node.wallet)

    yield node.connect()

    // Recover missed transactions
    //node.scan(spv.chain.height - 1);

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

  self.awaitTransaction = function(hash) {
    return watcher.watch(hash);
  }

  // Broadcast a signed transaction (settlements, refunds)
  // Accepts hex encoded string, Buffer or bcoin.TX
  // will try upto three times on pool.options.invTimeout
  self.broadcast = co.wrap(function* broadcast(tx) {

    if(!(tx instanceof bcoin.TX)) {
      tx = bcoin.TX.fromRaw(tx)
    }

    for(let tries = 0; tries < 3; tries++) {
      try {
        //using pool to do our own error handling
        yield node.pool.broadcast(tx)
        // success
        return
      } catch(e) {
        if(e.message == 'Timed out.')//reliable ?
          continue
        throw e
      }
    }

    throw new Error('timeout')
  })

  // self.resendPendingTransactions = function resendPendingTransactions() {
  //   // It resends transactions in the wallet with 0 confirmations
  //   // I don't think we need to expose this, it is done automatically when
  //   // the walletdb is opened
  //   return node.walletdb.resend()
  // }

  self.close = function close() {
    return node.close()
  }

  // Create, fund and sign a transaction from array of outputs
  // (commitments of a joystream contract)
  // https://github.com/bcoin-org/bcoin/pull/122 (disable sorting)
  self.send = function send(outputs) {
      //wallet.send (best option) (but it sorts outputs BIP69 when constructing a Tx.. we don't want that!)
      //using wallet.fund or wallet.sign can result in double spend (there is no utxo locking)

      // return the transaction (promise)
      return node.wallet.send({
        noSorting: true,
        outputs: outputs
      })
  }

  return self;
}

// shortcuts
SPVWallet.generatePrivateKey = function generatePrivateKey() {
  return bcoin.ec.generatePrivateKey();
}


//paychan store for buyer and tx pool for seller
