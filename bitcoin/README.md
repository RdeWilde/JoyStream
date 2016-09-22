
# joystream-bitcoin

An SPV wallet library consisting of three main modules: Store, SPVWallet and BlockCypherWallet. 

## Documentation

### Store
Store is a key, transaction and header chain store. It is BIP32/39/43/44 compliant, and supports
both P2SH and P2PKH. 

...coming later...

### SPVWallet 
SPVWallet is a SPV wallet, in the sense that it is a SPV client managing a Store, and the client is
the mSIGNA coinq client.

...coming later...

### BlockCypherWallet 
The BlockCypherWallet is an attempt at managing a Store using the Blockcypher API, but it was abandoned,
and will likely get dropped in the near future.

...coming later...

## Building

```
qmake bitcoin.pro
make
```

## Testing

Build and run ../test/spv-wallet

NB: Tests cannot to proper script validation, only signature validation, as the coincore (mSIGNA) has not built in script and interpeter support.
