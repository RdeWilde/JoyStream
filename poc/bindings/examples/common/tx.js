'use strict';
var assert = require('assert');

var Buffer = require('buffer').Buffer;

var Tx = require('../../').common.Transaction;

var tx1 = new Tx();

var buf = tx1.toBuffer();

var tx2 = new Tx(buf);

assert.deepEqual(buf, tx2.toBuffer());
