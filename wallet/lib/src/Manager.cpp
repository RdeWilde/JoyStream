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

#include <common/Network.hpp>
#include <common/KeyPair.hpp>
#include <common/TransactionId.hpp>
#include <common/Utilities.hpp>

#include <CoinCore/typedefs.h> // bytes_t
#include <CoinCore/CoinNodeData.h> // Coin::CoinBlockHeader, Transaction types: outpoints, transactions, ...
#include <CoinQ/CoinQ_script.h> // getAddressForTxOutScript

#include <QString>

namespace Wallet {

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
    db.setDatabaseName(QFileInfo(walletFile).fileName());

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

    // Build utxo
    //updateUtxoSet();
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
    // Working variables used in each query
    QSqlQuery query;
    QSqlError e;
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

/**
quint64 Wallet::nextHdIndex() {

    quint64 nextHdIndex;

    _mutex.lock();
    nextHdIndex = _nextIndex;
    _mutex.unlock();

    return nextHdIndex;
}
*/

Coin::PrivateKey Manager::issueKey() {
    return _issueKey()._privateKey;
}

Coin::P2PKHAddress Manager::getReceiveAddress() {
    return _createReceiveAddress()._address;
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

bool Manager::addBlockHeader(const Coin::CoinBlockHeader & blockHeader, quint64 numberOfTransactions, bool isOnMainChain, quint32 totalProofOfWork) {

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
    BlockHeader::Record record(blockHeader, numberOfTransactions, isOnMainChain, totalProofOfWork);

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

        _mutex.unlock(); // <--- may be possible to move up
        return true;
    }
}

bool Manager::calculateAndSetFee(const Coin::TransactionId & transactionId, quint64 & fee) {
    throw std::runtime_error("not implemented");
    emit feeFound(transactionId, fee);
}

Transaction::ChainMembership Manager::bestChainMemberShip(const Coin::TransactionId & transactionId) {
    throw std::runtime_error("not implemented");
}

/**
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

QList<UtxoCreated> getAllUtxoCreated() {
    throw std::runtime_error("not implemented");
}

QList<UtxoDestroyed> getAllUtxoDestroyed() {
    throw std::runtime_error("not implemented");
}

/**
void Wallet::releaseKeys(const QSet<Coin::KeyPair> & keys) {

    // for each key, if it is in dbase, but not in use, then place in key pool:

}

void Wallet::updateKeyPool() {

    // Net change to key pool
    quint32 diff = 0;

    _mutex.lock();

    // Scrap current pool
    _keyPool.clear();

    // Find all keys not currently used

    // Put in key pool

    _mutex.unlock();

    // Send signal of what changes were
    emit keyPoolUpdated(diff);
}
*/

quint64 Manager::updateUtxoSet() {

    quint64 utxoSetSize;

    _mutex.lock();

    // Scrap current utxo
    _utxo.clear();

    // Find all utxo

    // Put in utxo

    utxoSetSize = _utxo.size();
    _mutex.unlock();

    // Send signal of what changes were
    emit utxoUpdated(utxoSetSize);

    return utxoSetSize;
}

bool Manager::releaseUtxo(const Coin::typesafeOutPoint & o) {

    bool wasActuallyReleased;

    _mutex.lock();

    //wasActuallyReleased = _lockedOutPoints.remove(o);

    _mutex.unlock();

    return wasActuallyReleased;
}

void Manager::broadcast(const Coin::Transaction & tx) {

    _mutex.lock();

    // do something: may not need to be mutexed
    // save tx in wallet?

    _mutex.unlock();
}

 QSqlDatabase Manager::db() {
     return _db;
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
