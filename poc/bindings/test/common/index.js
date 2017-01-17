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

        it('default constructor', function(){
            var hash = new PubKeyHash();
            assert.equal(hash.toBuffer().length, 20);
        })

        it('construct from buffer', function(){
            var buf = new Buffer('01020405060708090A0B0C0D0E0F101112131415','hex');
            assert.equal(buf.length, 20);

            var hash = new PubKeyHash(buf);
            assert.deepEqual(hash.toBuffer(), buf);
        })

        it('constructor throws if buffer not correct size', function(){
            assert.throws(function(){
                new PubKeyHash(new Buffer(100));
            }, TypeError);
        })

        it('constructor throws if argument not a buffer', function(){
            assert.throws(function(){
                new PubKeyHash("string");
            }, TypeError)
        })
    })

    describe('PublicKey', function(){
        var PublicKey = nativeExtension.common.PublicKey;

        it('default constructor', function(){
            var pk = new PublicKey();
            assert.equal(pk.toBuffer().length, 33);
        })

        it('constructor throws if buffer not correct size', function(){
            assert.throws(function(){
                new PublicKey(new Buffer(100));
            }, TypeError);
        })

        it('constructor throws if argument not a buffer', function(){
            assert.throws(function(){
                new PublicKey("string");
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
