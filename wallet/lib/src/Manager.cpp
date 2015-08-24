/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#include <wallet/Manager.hpp>
#include <wallet/Key.hpp>
#include <wallet/Address.hpp>
#include <wallet/BlockHeader.hpp>
#include <wallet/Transaction.hpp>
#include <wallet/OutPoint.hpp>
#include <wallet/Input.hpp>
#include <wallet/TransactionHasInput.hpp>
#include <wallet/Output.hpp>
#include <wallet/TransactionHasOutput.hpp>
#include <wallet/InBoundPayment.hpp>
#include <wallet/OutBoundPayment.hpp>
#include <wallet/Payer.hpp>
#include <wallet/OuputFundsPayer.hpp>
#include <wallet/Slot.hpp>
#include <wallet/Payee.hpp>
#include <wallet/Metadata.hpp>
#include <wallet/UtxoCreated.hpp>
#include <wallet/UtxoDestroyed.hpp>
#include <wallet/SPVClient.hpp>

#include <common/Network.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/Utilities.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinCore/typedefs.h> // bytes_t
#include <CoinCore/CoinNodeData.h> // Coin::CoinBlockHeader, Transaction types: outpoints, transactions, ...
#include <CoinQ/CoinQ_script.h> // getAddressForTxOutScript
#include <CoinQ/CoinQ_blocks.h> // CoinQBlockTreeMem

#include <QString>
#include <QSqlRecord>
#include <QVariant>

namespace Wallet {

void Manager::test_routine() {
    std::cout << "void test_routine();";
}

QStringList Manager::dnsSeeds(Coin::Network network) {

    if(network == Coin::Network::mainnet) {

        return QStringList() << "seed.bitcoin.sipa.be"
                             << "dnsseed.bluematt.me"
                             << "dnsseed.bitcoin.dashjr.org"
                             << "seed.bitcoinstats.com"
                             << "bitseed.xf2.org"
                             << "seed.bitcoin.jonasschnelli.ch";

    } else if(network == Coin::Network::testnet3) {

        return QStringList() << "testnet-seed.alexykot.me"
                             << "testnet-seed.bitcoin.petertodd.org"
                             << "testnet-seed.bluematt.me"
                             << "testnet-seed.bitcoin.schildbach.de";
    } else
        Q_ASSERT(false);
}

QSqlDatabase Manager::openDatabaseConnection(const QString & walletFile) {

    // Create connection
    // We need non-default connection names, as testing will involve
    // running multiple clients which each start their own connections
    QSqlDatabase db;

    if(QSqlDatabase::connectionNames().contains(walletFile))
        db = QSqlDatabase::database(walletFile);
    else
        db = QSqlDatabase::addDatabase("QSQLITE", walletFile);

    Q_ASSERT(db.isValid());

    // Create database file
    db.setDatabaseName(walletFile);

    if(!db.open()) {

        // Build error message and throw exception
        QString errorMessage = "Could not open connection with database in file: " + walletFile;

        throw std::runtime_error(errorMessage.toStdString());
    }

    return db;

}

Manager::Manager(const QString & walletFile)
    : _mutex(QMutex::Recursive) // Same thread CAN aquire same lock multiple times, but must do parity unlocks
    , _walletFile(walletFile)
    , _db(Manager::openDatabaseConnection(walletFile))
    , _latestBlockHeight(0)
    , _lastComputedZeroConfBalance(0) {

    // If wallet does not exist, then create it
    if(!QFile(walletFile).exists()) {

        // Build error message
        QString errorMessage = "Was unable to read wallet file: " + walletFile;

        throw std::runtime_error(errorMessage.toStdString());
    }

    if(!validateWalletStructure(_db))
        throw std::runtime_error("Invalid wallet structure.");

    // Load metadata
    _network = Metadata::getNetwork(_db);
    _seed = Metadata::getSeed(_db);
    _created = Metadata::getCreated(_db);

    // Set keychain based on seed
    _keyChain = _seed.generateHDKeychain();

    // Build key pool
    //updateKeyPool();

    // We count the number of keys in the wallet
    // so that the hd index counter can be set
    try {

        // Exception is thrown if wallet is empty, otherwise
        // the biggest index is returned.
        _nextIndex = Key::maxIndex(_db) + 1;

    } catch(std::runtime_error & e) {
        _nextIndex = 0;
    }

    //
    //_latestBlockHeight =

    // Compute zero conf balance
    _lastComputedZeroConfBalance = balance(0);
    emit zeroConfBalanceChanged(_lastComputedZeroConfBalance);

    // Build utxo
    //updateUtxoSet();
}

Manager::~Manager(){

    // .stop, and delete all objects in _spvClients[host] = client;
}

void Manager::createNewWallet(const QString & walletFile, Coin::Network network, const Coin::Seed & seed) {

    // If the wallet file already exists, throw exception
    if(QFile::exists(walletFile)) {
        throw std::runtime_error("Cannot create new wallet, one already exists, please delete manually"); // : " + walletFile
    }

    // Open database connection
    QSqlDatabase db = Manager::openDatabaseConnection(walletFile);

    // Create metdata key-value store, and add rows for keys, and corresponding default values
    Metadata::createKeyValueStore(db, seed, network, QDateTime::currentDateTime());

    // Create relational tables
    bool ok;

    ok = Key::createTable(db);
    Q_ASSERT(ok);

    ok = Address::createTable(db);
    Q_ASSERT(ok);

    ok = BlockHeader::createTable(db);
    Q_ASSERT(ok);

    ok = OutPoint::createTable(db);
    Q_ASSERT(ok);

    ok = Input::createTable(db);
    Q_ASSERT(ok);

    ok = Output::createTable(db);
    Q_ASSERT(ok);

    ok = Transaction::createTable(db);
    Q_ASSERT(ok);

    ok = TransactionHasInput::createTable(db);
    Q_ASSERT(ok);

    ok = TransactionHasOutput::createTable(db);
    Q_ASSERT(ok);

    /**

      ===============================
      InBoundPayment::createTableQuery(db).exec()

      OutBoundPayment::createTableQuery(db).exec()

      ===============================
      Payer::createTableQuery(db).exec()

      OuputFundsPayer::createTableQuery(db).exec()

      Slot::createTableQuery(db).exec()

      Payee::createTableQuery(db).exec()
    */

    // Close connection: Is this requried? I think destructor does this anyway.
    db.close();
}

bool Manager::validateWalletStructure(QSqlDatabase & db) {
    return true;
}

void Manager::startSPVClient(const QString & blockHeaderStore, const QString & host) {

    if(_clients.contains(host))
        throw std::runtime_error("SPV client for host already exists.");

    // Pick default paramters for network being used
    CoinQ::NetworkSelector selector(_network == Coin::Network::testnet3 ? "testnet3" : "bitcoin");
    CoinQ::CoinParams coinParams = selector.getCoinParams();

    // Create new client
    CoinQ::Network::NetworkSync * client = new CoinQ::Network::NetworkSync(coinParams);

    // Add to map
    _clients[host] = client;

    // Get list of addresses and outpoints to add to bloom filter
    const QSet<Coin::P2PKHAddress> addresses = QSet<Coin::P2PKHAddress>(); // QList<Coin::P2PKHAddress> Manager::listReceiveAddresses()
    const QSet<Coin::typesafeOutPoint> outPoints = QSet<Coin::typesafeOutPoint>(); // ?

    // Create bloom filter and populate with all receive addresses
    // and relevant outpoints
    // NBNBNBNB!!!!: <--- is this filter copied or what?
    // addresses.size() + outPoints.size()
    Coin::BloomFilter filter(10, 0.001, 0, 0); // <-- factor out into settings class; //

    // Add addresses to the bloom filter
    for(QSet<Coin::P2PKHAddress>::const_iterator i = addresses.constBegin(),
        end = addresses.constEnd();
        i != end;
        i++);
        //filter.insert(*i);

    // Add addresses to the bloom filter
    for(QSet<Coin::typesafeOutPoint>::const_iterator i = outPoints.constBegin(),
        end = outPoints.constEnd();
        i != end;
        i++);
        //filter.insert(*i);

    // Set filter to be used by p2p client
    //client->setBloomFilter(filter);

    /**
     * Setup handlers for spv client events
     */

    // Load headers from block store
    std::string str1 = blockHeaderStore.toStdString();
    client->loadHeaders(str1, false, [&](const CoinQBlockTreeMem & blocktree) {

        //QMetaObject::invokeMethod(this, "blockStoreLoaded", Q_ARG(const CoinQBlockTreeMem &, blocktree));
        this->test_routine();

        return true; // !g_bShutdown; <--- what is this for?
    });

/**
    _spvP2PClient.subscribeStarted([&]() {
       std::cout << std::endl << " started." << std::endl;
    });

    _spvP2PClient.subscribeStopped([&]() {
       std::cout << std::endl << " stopped." << std::endl;
    });

    _spvP2PClient.subscribeOpen([&]() {
       std::cout << std::endl << " open." << std::endl;
    });

    _spvP2PClient.subscribeClose([&]() {
       std::cout << std::endl << " closed." << std::endl;
    });

    _spvP2PClient.subscribeTimeout([&]() {
       std::cout << std::endl << " timeout." << std::endl;
    });

    _spvP2PClient.subscribeConnectionError([&](const string& error, int code) {
       std::cout << std::endl << " connection error: " << error << std::endl;
    });

    _spvP2PClient.subscribeProtocolError([&](const string& error, int code) {
       std::cout << std::endl << " protocol error: " << error << std::endl;
    });

    _spvP2PClient.subscribeBlockTreeError([&](const string& error, int code) {
       std::cout << std::endl << " block tree error: " << error << std::endl;
    });

    _spvP2PClient.subscribeSynchingHeaders([&]() {
       std::cout << std::endl << " fetching headers." << std::endl;
    });

    _spvP2PClient.subscribeHeadersSynched([&]() {
       std::cout << std::endl << " headers synched." << std::endl;
       hashvector_t hashes;
       _spvP2PClient.syncBlocks(hashes, time(NULL) - 10*60*60); // Start 10 hours earlier
    });

    _spvP2PClient.subscribeSynchingBlocks([&]() {
       std::cout << std::endl << " fetching blocks." << std::endl;
    });

    _spvP2PClient.subscribeBlocksSynched([&]() {
       std::cout << std::endl << " blocks synched." << std::endl;

       std::cout << std::endl << "Fetching mempool..." << std::endl;
       _spvP2PClient.getMempool();
    });

    _spvP2PClient.subscribeStatus([&](const string& status) {
       std::cout << std::endl << " status: " << status << std::endl;
    });

    _spvP2PClient.subscribeNewTx([&](const Transaction& tx) {
       std::cout << std::endl << "NEW TX: " << tx.getHashLittleEndian().getHex() << std::endl;
    });

    _spvP2PClient.subscribeMerkleTx([&](const ChainMerkleBlock& merkleBlock, const Transaction& tx, unsigned int txIndex, unsigned int txTotal)
    {
       std::cout << "  tx (" << txIndex << "/" << (txTotal - 1) << "): " << tx.getHashLittleEndian().getHex() << std::endl;
    });

    _spvP2PClient.subscribeBlock([&](const ChainBlock& block) {
       std::cout << std::endl << "NEW BLOCK: " << block.blockHeader.getHashLittleEndian().getHex() << " height: " << block.height << std::endl;
    });

    _spvP2PClient.subscribeMerkleBlock([&](const ChainMerkleBlock& merkleblock) {

        std::cout << std::endl << "NEW MERKLE BLOCK" << std::endl
                    << "  hash: " << merkleblock.blockHeader.getHashLittleEndian().getHex() << std::endl
                    << "  height: " << merkleblock.height << std::endl;

        try
        {
            PartialMerkleTree tree(merkleblock.nTxs, merkleblock.hashes, merkleblock.flags, merkleblock.merkleRoot());
            std::vector<uchar_vector> txhashes = tree.getTxHashesLittleEndianVector();
            unsigned int i = 0;
            std::cout << "should contain txs:" << std::endl;
            for (auto& txhash: txhashes) { std::cout << "  tx " << i++ << ": " << uchar_vector(txhash).getHex() << std::endl; }
        }
        catch (const exception& e)
        {
           std::cout << "Error constructing partial merkle tree: " << e.what() << std::endl;
        }

        std::cout << "--------------------" << std::endl;
        blockTxIndex = 0;
    });

    _spvP2PClient.subscribeAddBestChain([&](const ChainHeader& header) {
       std::cout << std::endl << " added to best chain: " << header.getHashLittleEndian().getHex() << " height: " << header.height << std::endl;
    });

    _spvP2PClient.subscribeRemoveBestChain([&](const ChainHeader& header) {
       std::cout << std::endl << " removed from best chain: " << header.getHashLittleEndian().getHex() << " height: " << header.height << std::endl;
    });

    _spvP2PClient.subscribeBlockTreeChanged([&]() {
       std::cout << std::endl << " block tree changed." << std::endl;
    });
    */

    // Start client:
    std::string stdHost = host.toStdString();
    std::string port = coinParams.default_port();

    client->start(stdHost, port);
}

Coin::Network Manager::network() const {
    return _network;
}

QDateTime Manager::created() const {
    return _created;
}

Coin::Seed Manager::seed() const {
    return _seed;
}

quint64 Manager::lastComputedZeroConfBalance() {

    quint64 balance;

    _mutex.lock();
    balance = _lastComputedZeroConfBalance;
    _mutex.unlock();

    return balance;
}

Coin::PrivateKey Manager::issueKey() {
    //emit numberOfKeysInWalletChanged(quint64 numberOfKeysInWallet);
    return _issueKey()._privateKey;
}

Coin::P2PKHAddress Manager::getReceiveAddress() {

    // Get new address
    Coin::P2PKHAddress address = _createReceiveAddress()._address;

    // Send signal about it
    emit addressCreated(address);

    return address;
}

QList<Coin::P2PKHAddress> Manager::listReceiveAddresses() {

    /**
     *
     *
     *
     *
     */

    throw std::runtime_error("not implemented");

}

QList<Coin::KeyPair> Manager::issueKeyPairs(quint64 numberOfPairs) {

    // List of keys to return
    QList<Coin::KeyPair> keys;

    // Create each pair and add to list
    for(quint64 i = 0;i < numberOfPairs;i++) {

        // Get private key
        Coin::PrivateKey sk = issueKey();

        // Generate corresponding (compressd) public key
        Coin::PublicKey pk = sk.toPublicKey();

        // Add key pair to list
        keys.append(Coin::KeyPair(pk, sk));
    }

    return keys;
}

quint64 Manager::numberOfKeysInWallet() {
    return Key::numberOfKeysInWallet(_db);
}

bool Manager::addBlockHeader(const Coin::CoinBlockHeader & blockHeader, quint64 numberOfTransactions, bool isOnMainChain, quint32 totalProofOfWork, quint64 blockHeight) {

    // Create private key to check if record exists
    BlockHeader::PK pk(blockHeader.getHashLittleEndian());

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(BlockHeader::exists(_db, pk)) {

        _mutex.unlock();
        return false;
    }

    // Create record
    BlockHeader::Record record(blockHeader, numberOfTransactions, isOnMainChain, totalProofOfWork, blockHeight);

    // Insert record
    bool ok = BlockHeader::insert(_db, record);
    Q_ASSERT(ok);

    // Signal event
    emit blockHeaderAdded(blockHeader); // drop the extra stuff!!

    _mutex.unlock();
    return true;
}

bool Manager::addOutPoint(const Coin::OutPoint & outPoint) {

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(OutPoint::exists(_db, outPoint)) {

        _mutex.unlock();
        return false;
    }

    // Insert record
    bool ok = OutPoint::insert(_db, OutPoint::PK(outPoint));
    Q_ASSERT(ok);

    // Signal event
    emit outPointAdded(outPoint);

    _mutex.unlock();
    return true;
}

bool Manager::addInput(const Coin::TxIn & txIn) {

    // Lets blindly attempt to add outpoint, it may or may not work
    addOutPoint(txIn.previousOut);

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // We are done if it already exists
    if(Input::exists(_db, txIn)) {

        _mutex.unlock();
        return false;
    }

    // Insert record
    bool ok = Input::insert(_db, Input::PK(txIn));
    Q_ASSERT(ok);

    // Signal event
    emit inputAdded(txIn);

    _mutex.unlock();
    return true;
}

bool Manager::addOutput(const Coin::TxOut & txOut) {

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    // Create primary key for output
    Output::PK pk(txOut.value, Coin::toByteArray(txOut.scriptPubKey));

    if(Output::exists(_db, pk)) {
        _mutex.unlock();
        return false;
    }

    // Get corresponding record for address corresponding to output script, if it exists
    Address::Record record;
    bool outputAddressIsControlledByWallet = Manager::getAddressForOutput(txOut, record);

    // Create output record
    Output::Record outputRecord(pk, outputAddressIsControlledByWallet ? QVariant(record._keyIndex) : QVariant());

    // Insert it
    bool ok = Output::insert(_db, outputRecord);
    Q_ASSERT(ok);

    // Signal event
    emit outputAdded(txOut);

    /**
    // Calculate fee forany transaction which may be depending on this
    // IF IT WAS ADDED, CALL FEE CALCULATION ROUTINE
    // // 4. fee re-calculation for any OTHER transaction  IF: !noOutputAdded -> this may have lead to us being able to
    //         quint64 fee;
    //    FOR:
    //    calculateAndSetFee(transaction.getHashLittleEndian(), fee);
    */

    _mutex.unlock();
    return true;
}

bool Manager::addTransaction(const Coin::Transaction & transaction) {

    // Lock so that checking existence and insertion are atomic
    _mutex.lock();

    Coin::TransactionId transactionId(transaction.getHashLittleEndian());

    // If the transaction has already been added, we are done
    if(Transaction::exists(_db, transactionId)) {
        _mutex.unlock();
        return false;
    } else {

        // Add transaction
        bool added = Transaction::insert(_db, Transaction::Record(transaction, QDateTime::currentDateTime())); // Change later
        Q_ASSERT(added);

        // Send signal
        emit transactionAdded(transaction);

        // Add inputs
        for(quint32 i = 0; i < transaction.inputs.size();i++) {

            // Get input and add
            Coin::TxIn in = transaction.inputs[i];
            addInput(in);

            // Add TransactionHasInput
            TransactionHasInput::insert(_db, TransactionHasInput::Record(TransactionHasInput::PK(transactionId, i), in));

            // =================================
            // REGISTER UTXO DESTRUCTION EVENT
            // REMOVE FROM UTXO (update balances)
            // =================================
        }

        // Add outputs
        for(quint32 i = 0; i < transaction.outputs.size();i++) {

            // Get output and add
            Coin::TxOut out = transaction.outputs[i];
            addOutput(out);

            // Add TransactionHasOutput
            TransactionHasOutput::insert(_db, TransactionHasOutput::Record(TransactionHasOutput::PK(transactionId, i), out));

            // =============================
            // REGISTER UTXO CREATION EVENT
            // ADD TO UTXO (update balances)
            // =============================
        }

        // Tries to calculate and set fee
        quint64 fee;
        //calculateAndSetFee(transactionId, fee);

        /////////////////////////
        // Recompute balance zero conf balance
        // WARNINGS: should really only consider effect of this one tx, in
        // terms of destroying and creating utxos. But do it later.
        /////////////////////////
        quint64 newBalance = balance(0);

        if(newBalance != _lastComputedZeroConfBalance) {

            // Send signal
            emit zeroConfBalanceChanged(newBalance);

            // Save new balance
            _lastComputedZeroConfBalance = newBalance;
        }

        _mutex.unlock(); // <--- may be possible to move up
        return true;
    }
}

/**
bool Manager::calculateAndSetFee(const Coin::TransactionId & transactionId, quint64 & fee) {
    throw std::runtime_error("not implemented");
    emit feeFound(transactionId, fee);
}

Transaction::ChainMembership Manager::bestChainMemberShip(const Coin::TransactionId & transactionId) {
    throw std::runtime_error("not implemented");
}


bool Manager::registerTransactionInBlock(const Coin::TransactionId  & transactionId, const Coin::BlockId & blockId, .. merkle proof) {

}
*/

Coin::Transaction Manager::getTransaction(const Coin::TransactionId & transactionId) {

    // Get transaction body
    Transaction::Record txRecord;
    bool exists = Transaction::exists(_db, transactionId, txRecord);

    // Throw exception if transaction is not in wallet, make type safe exception later
    if(!exists)
        throw std::runtime_error("Transaction does not exist in wallet");

    Coin::Transaction tx = txRecord.toTransaction();

    // Get inputs
    tx.inputs = TransactionHasInput::inputsOfTransaction(_db, transactionId);

    // Get outputs
    tx.outputs = TransactionHasOutput::outputsOfTransaction(_db, transactionId);

    return tx;
}

QList<Coin::Transaction> Manager::allTransactions() {
    throw std::runtime_error("not implemented");
}

quint64 Manager::numberOfTransactions() {
    return Transaction::getTransactionCount(_db);
}

QList<Coin::UnspentP2PKHOutput> Manager::listUtxo(quint64 minimalConfirmations) {

     QList<Coin::UnspentP2PKHOutput> list;

     // If we don't want zero conf, then we have to join with blockheader
     if(minimalConfirmations > 0) {

         throw std::runtime_error("not implemented");

         // Determinest lower block we care about
         quint64 lowestBlockHeightOfInterest = _latestBlockHeight - minimalConfirmations;

     } else {

         // Semantics: return all outputs controlled by wallet which are not spent by any other transaction
         // we know about
        QString sql = "SELECT "
                      "   [Transaction].transactionId, "
                      "   [Transaction].seen, "
                      "   TransactionHasOutput.[index], "
                      "   TransactionHasOutput.value, "
                      "   TransactionHasOutput.scriptPubKey, "
                      "   [Key].privateKey, "
                      "   Output.keyIndex "
                      "   "
                      " FROM "
                      "   [Transaction] JOIN "
                      "   TransactionHasOutput JOIN "
                      "   Output JOIN "
                      "   [Key] "
                      " ON "
                      "   [Transaction].transactionId = TransactionHasOutput.transactionId AND "
                      "   TransactionHasOutput.value = Output.value AND "
                      "   TransactionHasOutput.scriptPubKey = Output.scriptPubKey AND "
                      "   Output.keyIndex IS NOT NULL AND "
                      "   [Key].[index] = Output.keyIndex "
                      " WHERE NOT EXISTS ( "
                      " SELECT "
                      "   * "
                      " FROM "
                      "   TransactionHasInput "
                      " WHERE "
                      "   TransactionHasInput.outPointTransactionId = TransactionHasOutput.transactionId AND "
                      "   TransactionHasInput.outPointOutputIndex = TransactionHasOutput.[index] "
                      ") ";


        // Perform query
        QSqlQuery query(sql, _db);

        Q_ASSERT(query.lastError().type() == QSqlError::NoError);

        while(query.next()) {

            // Turn record into unspent output
            QSqlRecord record = query.record();

            Coin::TransactionId transactionId(record.value("transactionId").toByteArray());
            QDateTime seen = record.value("seen").toDateTime();

            bool ok;
            quint32 index = record.value("index").toUInt(&ok);
            Q_ASSERT(ok);

            quint64 value = record.value("value").toULongLong(&ok);
            Q_ASSERT(ok);

            QByteArray scriptPubKey = record.value("scriptPubKey").toByteArray();
            Coin::PrivateKey privateKey(record.value("privateKey").toByteArray());

            // Add to list
            list.append(Coin::UnspentP2PKHOutput(Coin::KeyPair(privateKey.toPublicKey(), privateKey),
                                                 Coin::typesafeOutPoint(transactionId,index),
                                                 value));
        }

     }

     return list;
}

QList<Coin::UnspentP2PKHOutput> Manager::lockUtxo(quint64 minimalAmount, quint64 minimalConfirmations) {

     QList<Coin::UnspentP2PKHOutput> list;

     // lock so now one else tries to claim utxo members at the same time
     _mutex.lock();

     // Get utxo
     QList<Coin::UnspentP2PKHOutput> utxo = listUtxo(minimalConfirmations);

     // Add up as many as possible to make balance
     // Needs to be radically improved by balancing three contradictory objectives
     // - use as few utxos as possible to lower size of spending tx, and thus lowering requird fee
     // - exceed required quantity with the least amount to avoid having to have a change output.
     // - optimize for coinage spending to minimize feess
     QSet<Coin::typesafeOutPoint> lockingCandidateOutPoints;
     quint64 currentBalance = 0;

     for(QList<Coin::UnspentP2PKHOutput>::const_iterator i = utxo.constBegin(),
         end = utxo.constEnd();
         i != end;
         i++) {

         // get unspent output
         Coin::UnspentP2PKHOutput unspent = *i;

         // Disregard if it is already locked
         if(_lockedOutPoints.contains(unspent.outPoint()))
            continue;

         // Otherwise use this output
         currentBalance += unspent.value();
         list.append(unspent);
         lockingCandidateOutPoints.insert(unspent.outPoint());

         if(currentBalance >= minimalAmount) {

             // lock all utxo used
             _lockedOutPoints.unite(lockingCandidateOutPoints);
             break;
         }
     }
     _mutex.unlock();

     // If we did not make full balance, then return empty list
     if(currentBalance < minimalAmount)
         list.clear();

     return list;
 }

QList<UtxoCreated> Manager::getAllUtxoCreated(quint64 minimalConfirmations) {

    QList<UtxoCreated> list;

    // If we don't want zero conf, then we have to join with blockheader
    if(minimalConfirmations > 0) {

        throw std::runtime_error("not implemented");

        // Determinest lower block we care about
        quint64 lowestBlockHeightOfInterest = _latestBlockHeight - minimalConfirmations;

    } else {

        // Semantics: return all outputs controlled by wallet which are not spent by any other transaction
        // we know about
       QString sql = "SELECT "
                     "   [Transaction].transactionId, "
                     "   [Transaction].seen, "
                     "   TransactionHasOutput.[index], "
                     "   TransactionHasOutput.value, "
                     "   TransactionHasOutput.scriptPubKey, "
                     "   [Key].privateKey, "
                     "   Output.keyIndex "
                     "   "
                     " FROM "
                     "   [Transaction] JOIN "
                     "   TransactionHasOutput JOIN "
                     "   Output JOIN "
                     "   [Key] "
                     " ON "
                     "   [Transaction].transactionId = TransactionHasOutput.transactionId AND "
                     "   TransactionHasOutput.value = Output.value AND "
                     "   TransactionHasOutput.scriptPubKey = Output.scriptPubKey AND "
                     "   Output.keyIndex IS NOT NULL AND "
                     "   [Key].[index] = Output.keyIndex ";

       // Perform query
       QSqlQuery query(sql, _db);

       Q_ASSERT(query.lastError().type() == QSqlError::NoError);

       while(query.next()) {

           // Turn record into unspent output
           QSqlRecord record = query.record();

           Coin::TransactionId transactionId(record.value("transactionId").toByteArray());
           QDateTime seen = record.value("seen").toDateTime();

           bool ok;
           quint32 index = record.value("index").toUInt(&ok);
           Q_ASSERT(ok);

           quint64 value = record.value("value").toULongLong(&ok);
           Q_ASSERT(ok);

           QByteArray scriptPubKey = record.value("scriptPubKey").toByteArray();
           Coin::PrivateKey privateKey(record.value("privateKey").toByteArray());

           // Add to list
           Coin::UnspentP2PKHOutput utxo(Coin::KeyPair(privateKey.toPublicKey(), privateKey), Coin::typesafeOutPoint(transactionId, index), value);
           list.append(UtxoCreated(utxo, seen));
       }

    }

    return list;
}

QList<UtxoDestroyed> Manager::getAllUtxoDestroyed(quint64 minimalConfirmations) {

    QList<UtxoDestroyed> list;

    // If we don't want zero conf, then we have to join with blockheader
    if(minimalConfirmations > 0) {

        throw std::runtime_error("not implemented");

        // Determinest lower block we care about
        quint64 lowestBlockHeightOfInterest = _latestBlockHeight - minimalConfirmations;

    } else {

        // Semantics: return all outputs controlled by wallet which are not spent by any other transaction
        // we know about
       QString sql = "SELECT "
                     "   [Transaction].seen, "
                     "   TransactionHasInput.transactionId, "
                     "   TransactionHasInput.[index], "
                     "   TransactionHasInput.outPointTransactionId, "
                     "   TransactionHasInput.outPointOutputIndex, "
                     "   TransactionHasInput.scriptSig, "
                     "   TransactionHasInput.sequence, "
                     "   TransactionHasOutput.value, "
                     "   TransactionHasOutput.scriptPubKey "
                     " FROM "
                     "   [Transaction] JOIN "
                     "   TransactionHasInput JOIN "
                     "   TransactionHasOutput JOIN "
                     "   Output "
                     " ON "
                     "   [Transaction].transactionId = TransactionHasInput.transactionId AND "
                     "   TransactionHasInput.outPointTransactionId = TransactionHasOutput.transactionId AND "
                     "   TransactionHasInput.outPointOutputIndex = TransactionHasOutput.[index] AND "
                     "   Output.value = TransactionHasOutput.value AND "
                     "   Output.scriptPubKey = TransactionHasOutput.scriptPubKey AND "
                     "   Output.keyIndex IS NOT NULL";

       // Perform query
       QSqlQuery query(sql, _db);

       Q_ASSERT(query.lastError().type() == QSqlError::NoError);

       while(query.next()) {

           bool ok; // integer conversion result indicator

           // Turn record into unspent output
           QSqlRecord record = query.record();

           QDateTime seen = record.value("seen").toDateTime();
           Coin::TransactionId spendingTx(record.value("transactionId").toByteArray());

           quint32 indexOfInputInSpendingTx = record.value("index").toUInt(&ok);
           Q_ASSERT(ok);

           Coin::TransactionId outPointTransactionId(record.value("outPointTransactionId").toByteArray());
           quint32 outPointOutputIndex = record.value("outPointOutputIndex").toUInt(&ok);
           //Q_ASSERT(ok);

           QByteArray scriptSig = record.value("scriptSig").toByteArray();
           quint32 sequence = record.value("sequence").toUInt(&ok);
           Q_ASSERT(ok);

           quint64 value = record.value("value").toULongLong(&ok);
           Q_ASSERT(ok);

           QByteArray scriptPubKey = record.value("scriptPubKey").toByteArray();

           // Create UtxoDestroyed members
           Coin::OutPoint outpoint(outPointTransactionId.toUCharVector(), outPointOutputIndex);
           Coin::TxIn inputInSpendingTx(outpoint, Coin::toUCharVector(scriptSig), sequence);

           Coin::TxOut oldUtxo(value, Coin::toUCharVector(scriptPubKey));

           // Add to list
           Wallet::UtxoDestroyed utxoDestroyed(spendingTx,
                                               seen,
                                               indexOfInputInSpendingTx,
                                               inputInSpendingTx,
                                               oldUtxo);

           list.append(utxoDestroyed);
       }

    }

    return list;
}

quint64 Manager::balance(quint64 minimalConfirmations) {

    quint64 balance = 0;

    // Find utxo
    QList<Coin::UnspentP2PKHOutput> list = listUtxo(minimalConfirmations);

    // Iterate
    for(QList<Coin::UnspentP2PKHOutput>::const_iterator i = list.constBegin(),
        end = list.constEnd();
        i != end;
        i++)
        // count towards total balance
        balance += (*i).value();

    return balance;
}

bool Manager::releaseUtxo(const Coin::typesafeOutPoint & o) {

    bool wasActuallyReleased;

    _mutex.lock();

    wasActuallyReleased = _lockedOutPoints.remove(o);

    if(wasActuallyReleased)
        emit utxoReleased(o);

    _mutex.unlock();

    return wasActuallyReleased;
}

void Manager::broadcast(const Coin::Transaction & tx) {

    throw std::runtime_error("not implemented");

    _mutex.lock();

    // do something: may not need to be mutexed
    // save tx in wallet?

    _mutex.unlock();
}

 QSqlDatabase Manager::db() {
     return _db;
 }


 void Manager::blockStoreLoaded(const CoinQBlockTreeMem& blocktree) {
    std::cout << "Best height: " << blocktree.getBestHeight() << " Total work: " << blocktree.getTotalWork().getDec() << std::endl;
 }


Address::Record Manager::_createReceiveAddress() {

    // Generate fresh wallet key
    Key::Record keyRecord = _issueKey();

    // Get private key
    Coin::PrivateKey sk = keyRecord._privateKey;

    // Get corresponding public key
    Coin::PublicKey pk = sk.toPublicKey();

    // Create wallet address
    Address::Record addressRecord(keyRecord._index, Coin::P2PKHAddress(_network, pk));

    // Insert into wallet
    Address::insert(_db, addressRecord);

    return addressRecord;
}

Key::Record Manager::_issueKey() {

    /**
     * ==========================================
     * UPDATE IN THE FUTURE TO WORK WITH KEY POOL
     * ==========================================
     */

    // We aquire lock so we protect our value of _nextIndex
    _mutex.lock();

    // Generate a new key by
    bytes_t rawPrivateKey = _keyChain.getPrivateSigningKey(_nextIndex);
    Coin::PrivateKey key = Coin::PrivateKey(rawPrivateKey);

    // Create wallet key entry
    Key::Record record(_nextIndex, key, QDateTime::currentDateTime(), true);

    // Add to table
    Key::insert(_db, record);
    Q_ASSERT(_nextIndex == Key::maxIndex(_db));

    // Increment to next index
    _nextIndex++;

    _mutex.unlock();

    return record;
}

bool Manager::getAddressForOutput(const Coin::TxOut & txOut, Address::Record & record) {

    // Deduce address from output script
    QString base58CheckEncodedString = QString::fromStdString(CoinQ::Script::getAddressForTxOutScript(txOut.scriptPubKey, Coin::networkToAddressVersions(_network)));

    // Get address from base58checkencoded string
    try {

        Coin::P2PKHAddress address = Coin::P2PKHAddress::fromBase58CheckEncoding(base58CheckEncodedString);
        Q_ASSERT(address.network() == _network);

        return Address::findFromAddress(_db, address, record);

    } catch (std::runtime_error & e) {

        // Wasn't valid p2pkh address (e.g p2sh)
        return false;
    }
}

}
