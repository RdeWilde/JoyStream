var nativeExtension = require('../../');
var assert = require('assert');
var Buffer = require('buffer').Buffer;

  describe('common', function() {
    it('exports', function(){
      assert.equal(typeof nativeExtension.common, 'object');
      assert.equal(typeof nativeExtension.common.Transaction, 'function');
      assert.equal(typeof nativeExtension.common.PrivateKey, 'function');
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

        it('generate', function(){
            var sk = PrivateKey.generate()
            assert(sk.valid())
        })

        it('toBuffer', function(){
            var sk = PrivateKey.generate()
            assert(sk.toBuffer().length, 32)
        })

    })

    xit('KeyPair', function() {
        assert.equal(typeof nativeExtension.common.KeyPair, 'function');
    })

    xit('PubKeyHash', function() {
        assert.equal(typeof nativeExtension.common.PubKeyHash, 'function');
    })
  })
