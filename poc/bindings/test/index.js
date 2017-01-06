var nativeExtension = require('../');
var assert = require('assert');

describe('native extension', function() {
    it('exported', function(){
      assert.equal(typeof nativeExtension.common, 'object');
    })
})
