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

        it('toBuffer', function() {
            var tx1 = new Tx();
            assert(typeof tx1.toBuffer, 'function');
            var buf = tx1.toBuffer();
            var tx2 = new Tx(buf);
            assert.deepEqual(buf, tx2.toBuffer());
        })

        it('version', function(){
            var tx = new Tx();
            assert.equal(1, tx.version());
        })

        it('locktime', function(){
            var tx = new Tx();
            assert.equal(0, tx.locktime());
        })
    })

    describe('PrivateKey', function() {
        var PrivateKey = nativeExtension.common.PrivateKey;

        var hexKey = "01020405060708090A0B0C0D0E0F10111213141501020405060708090A0B0C0D";
        assert.equal(hexKey.length, 64);

        it('default constructor', function(){
            var sk = new PrivateKey();
            assert.equal(sk.toBuffer().length, 32);
            assert.deepEqual(sk.toBuffer(), new Buffer(32).fill(0));
        })

        it('construct from buffer', function(){
            var sk = new PrivateKey(new Buffer(hexKey, 'hex'));
            assert.equal(sk.toBuffer().length, 32);
            assert.deepEqual(sk.toBuffer(), new Buffer(hexKey, 'hex'));
        })

        it('constructor throws if buffer not correct size', function(){
            assert.throws(function(){
                new PrivateKey(new Buffer(100));
            }, TypeError);
        })

        it('constructor throws if argument not a buffer', function(){
            assert.throws(function(){
                new PrivateKey("string");
            }, TypeError)
        })

        it('generate', function(){
            var sk = PrivateKey.generate()
            assert(sk.valid())
            assert.equal(sk.toBuffer().length, 32)
        })

        it('toBuffer', function(){
            var sk = PrivateKey.generate()
            assert.equal(sk.toBuffer().length, 32)
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
