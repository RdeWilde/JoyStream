var nativeExtension = require('../../');
var assert = require('assert');
var Buffer = require('buffer').Buffer;

  describe('common', function() {
    it('exports', function(){
      assert.equal(typeof nativeExtension.common.Transaction, 'function');
      assert.equal(typeof nativeExtension.common.PrivateKey, 'function');
      assert.equal(typeof nativeExtension.common.PubKeyHash, 'function');
      assert.equal(typeof nativeExtension.common.PublicKey, 'function');
      assert.equal(typeof nativeExtension.common.OutPoint, 'function');
      assert.equal(typeof nativeExtension.common.TransactionId, 'function');
    });

    describe('Transaction', function() {
        var Tx = nativeExtension.common.Transaction;

        it('fromObject', function() {
            //smallest possible transaction data
            // version:4bytes, inputs: 1 bytes, outputs:1 byte, locktime: 4bytes
            var buf = new Buffer(10).fill(0);
            var tx1 = new Tx({
                tx: buf
            })

            assert(tx1 instanceof Tx);

            assert(tx1.tx instanceof Buffer);
            assert.deepEqual(tx1.tx, buf);

            // modifying original buffer does not affect our copy
            buf.fill(1);
            assert.notDeepEqual(tx1.tx, buf);
        })

        it('throws TypeError when missing argument', function(){
            assert.throws(function(){
                new Tx();
            }, TypeError)
        })

        it('throws TypeError on invalid argument', function(){
            assert.throws(function(){
                new Tx({});
            }, TypeError)
        })

        it('throws TypeError on invalid transaction data', function(){
            assert.throws(function(){
                new Tx({
                    tx: new Buffer(2).fill(0)
                });
            }, TypeError)
        })

        it('make a copy', function(){
            var buf = new Buffer(10).fill(0);
            var tx1 = new Tx({
                tx: buf
            })

            var tx2 = new Tx(tx1);
            assert.deepEqual(tx1, tx2);
        })
    })

    describe('PrivateKey', function() {
        var PrivateKey = nativeExtension.common.PrivateKey;

        var hexKey = "01020405060708090A0B0C0D0E0F10111213141501020405060708090A0B0C0D";
        assert.equal(hexKey.length, 64)

        it('fromObject', function(){
            var sk = new PrivateKey({
                sk: new Buffer(hexKey, 'hex')
            });

            assert(sk instanceof PrivateKey)
            assert(sk.sk instanceof Buffer)

            assert.equal(sk.sk.length, 32)
            assert.deepEqual(sk.sk, new Buffer(hexKey, 'hex'))
        })

        it('throws TypeError if buffer not correct size', function(){
            assert.throws(function(){
                new PrivateKey({
                    sk: new Buffer(100)
                })
            }, TypeError)
        })

        it('throws TypeError on invalid argument', function(){
            assert.throws(function(){
                new PrivateKey({
                    sk: "notabuffer"
                })
            }, TypeError)
        })

        it('generates new key if no argument provided', function(){
            var sk = new PrivateKey();
            assert(sk instanceof PrivateKey)
            assert(sk.sk instanceof Buffer)
            assert.equal(sk.sk.length, 32)
        })

    })

    describe('PubKeyHash', function() {
        var PubKeyHash = nativeExtension.common.PubKeyHash;

        it('fromObject', function(){
            var buf = new Buffer('01020405060708090A0B0C0D0E0F101112131415','hex');
            assert.equal(buf.length, 20)

            var hash = new PubKeyHash({
                hash: buf
            })

            assert(hash instanceof PubKeyHash)
            assert(hash.hash instanceof Buffer)
            assert.deepEqual(hash.hash, buf)
        })

        it('throws TypeError when missing argument', function(){
            assert.throws(function(){
                new PubKeyHash();
            }, TypeError)
        })

        it('throws TypeError if buffer not correct size', function(){
            assert.throws(function(){
                new PubKeyHash({
                    sk: new Buffer(100)
                })
            }, TypeError)
        })

        it('throws TypeError on invalid argument', function(){
            assert.throws(function(){
                new PubKeyHash({
                    sk: "notabuffer"
                })
            }, TypeError)
        })
    })

    describe('PublicKey', function(){
        var PublicKey = nativeExtension.common.PublicKey;
        
        // just a random valid public key
        var pubKey = new Buffer("03897cdec980767acd0c39ff17287cab606ab6d53851d942eb3fcfa5fff05494ad", "hex")

        it('fromObject', function(){
            var pk = new PublicKey({
                pk: pubKey
            });

            assert(pk instanceof PublicKey)
            assert(pk.pk instanceof Buffer)

            assert.equal(pk.pk.length, 33)
            assert.deepEqual(pk.pk, pubKey)
        })

        it('throws TypeError when missing argument', function(){
            assert.throws(function(){
                new PublicKey();
            }, TypeError)
        })

        it('throws TypeError if buffer not correct size', function(){
            assert.throws(function(){
                new PublicKey({
                    pk: new Buffer(100)
                })
            }, TypeError)
        })

        it('throws TypeError on invalid argument', function(){
            assert.throws(function(){
                new PublicKey({
                    wrongname: "notabuffer"
                })
            }, TypeError)
        })

    })

    describe('TransactionId', function(){
        var TxId = nativeExtension.common.TransactionId;
        
        // just a random valid public key
        var id = new Buffer(32);

        it('fromObject', function(){
            var txid = new TxId({
                txid: id
            });

            assert(txid instanceof TxId)
            assert(txid.txid instanceof Buffer)

            assert.equal(txid.txid.length, 32)
            assert.deepEqual(txid.txid, id)
        })

        it('throws TypeError when missing argument', function(){
            assert.throws(function(){
                new TxId();
            }, TypeError)
        })

        it('throws TypeError if buffer not correct size', function(){
            assert.throws(function(){
                new TxId({
                    pk: new Buffer(100)
                })
            }, TypeError)
        })

        it('throws TypeError on invalid argument', function(){
            assert.throws(function(){
                new TxId({
                    wrongname: "notabuffer"
                })
            }, TypeError)
        })

    })

    describe('OutPoint', function(){
        var OutPoint = nativeExtension.common.OutPoint;

        it('default constructor', function(){
            var op = new OutPoint();
        })
    })
  })
