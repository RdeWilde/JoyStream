var nativeExtension = require('../');
var assert = require('assert');

describe('native extension', function() {
    it('common submodule', function(){
      assert.equal(typeof nativeExtension.common, 'object');
    })
})
