/**
 * Created by bedeho on 28/02/17.
 */

'use strict'

const NativeExtension = require('../../build/Release/NativeExtension.node');
const Node = require('../../dist/node');

var debug = require('debug')('node');
var fs = require('fs');
const path = require('path');
const rimraf = require('rimraf');
/// SETUP ================================================

// Parameters
var buyer_port = 7123;
var seller_port = 9292;
var work_directory_path = __dirname + path.sep + "integration_test_workdir";
var torrent_path = __dirname + path.sep + "torrent" + path.sep;
var torrent_file_path = torrent_path + "14405804O1_201507_archive.torrent";
var torrent_data_path = torrent_path + "14405804O1_201507" + path.sep;

// Cleanup and recreate work directory
if(fs.existsSync(work_directory_path))
    rimraf.sync(work_directory_path);
else
    fs.mkdirSync(work_directory_path);

// Create torrent_info
var torrent_info = new NativeExtension.TorrentInfo(torrent_file_path);

/// SELLER  ================================================

//var seller = new lib.Joystream(seller_port);

var seller = new Node(seller_port);

var seller_pack = {
    params : {
        ti: torrent_info,
        savePath: torrent_data_path
    },
    terms : { // //  50, 1, 10, 15000, 5000
        minPrice: 50,
        minLock: 1,
        maxNumberOfSellers: 10,
        minContractFeePerKb: 15000,
        settlementFee: 5000
    }
};

// Add torrent
seller.plugin.addTorrent(seller_pack.params, function(err, res) { });

seller.on('add_torrent_alert', function(torrent) {

    console.log("Here we finally have torrent object");

    torrent.on('')

    seller.plugin.to_sell_mode(info_hash, seller_pack.terms, function(err, res) {

        if(err) {
            debug("seller: Going to sell mode failed.");
            return;
        } else
            debug("seller: Went to sell mode.");




        seller.plugin.start(info_hash, function(err) {

            if(err) {
                debug("seller: Starting torrent plugin failed.");
                return;
            } else
                debug("seller: Torrent plugin started.");

            // something


        });

    });



});

// failure? how to detect, not possible



// Go to sell mode
/**

 */