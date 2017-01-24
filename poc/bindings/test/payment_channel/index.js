var ext = require('../../')
var assert = require('assert')
var Buffer = require('buffer').Buffer
var bcoin = require('bcoin')

describe('paymentchannel', function() {
    // exported functions
    assert.equal(typeof ext.commitmentToOutput, 'function');

    let sk = bcoin.ec.generatePrivateKey()
    let pk = bcoin.ec.publicKeyCreate(sk) // compressed by default

    describe('commitmentToOutput', function(){

      it('construct from commitment', function(){
        let output = ext.commitmentToOutput({
          value: 1000,
          payor: pk,
          payee: pk,
          locktime: 10
        })

        assert(output instanceof Buffer)
      })

      it('throws TypeError on conversion failures', function(){
        assert.throws(function(){
          ext.commitmentToOutput()
        }, TypeError)

        assert.throws(function(){
          ext.commitmentToOutput({value: null})
        }, TypeError)

        assert.throws(function(){
          ext.commitmentToOutput({value: 1, payor: null})
        }, TypeError)

        assert.throws(function(){
          ext.commitmentToOutput({value: 1, payor: pk, payee: null})
        }, TypeError)

        assert.throws(function(){
          ext.commitmentToOutput({value: 1, payor: pk, payee: pk, locktime: null })
        }, TypeError)

      })
    })
})
