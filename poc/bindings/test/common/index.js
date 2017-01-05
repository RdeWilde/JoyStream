var nativeExtension = require('../../');
var assert = require('assert');
var Buffer = require('buffer').Buffer;

  describe('common', function() {
    it('namespace exists', function(){
      assert.equal(typeof nativeExtension.common, 'object');
    });

    describe('Transaction', function() {
        var Tx;

        it('constructor', function() {
          assert.equal(typeof nativeExtension.common.Transaction, 'function');
        })

        Tx = nativeExtension.common.Transaction;

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

    xit('PrivateKey', function() {
        assert.equal(typeof nativeExtension.common.PrivateKey, 'function');
    })

    xit('KeyPair', function() {
        assert.equal(typeof nativeExtension.common.KeyPair, 'function');
    })

    xit('PubKeyHash', function() {
        assert.equal(typeof nativeExtension.common.PubKeyHash, 'function');
    })
  })
