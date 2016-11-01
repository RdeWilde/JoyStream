#include <app_kit/TransactionSendQueue.hpp>

namespace joystream {
namespace appkit {

TransactionSendQueue::TransactionSendQueue(bitcoin::SPVWallet *wallet, const Interval minFlushInterval)
    : _wallet(wallet),
      _minFlushInterval(minFlushInterval) {

    // Transaction was successfully broadcast and we have received it from the peer
    QObject::connect(_wallet, &bitcoin::SPVWallet::txUpdated, [this](Coin::TransactionId txid, int confirmations) {
        if(_transactionsWaitingNotification.find(txid) != _transactionsWaitingNotification.end()) {

            if(_transactionsWaitingNotification[txid].success)
                _transactionsWaitingNotification[txid].success();

            _transactionsWaitingNotification.erase(txid);
        }

    });

    //We need feedback from the wallet when a transaction is not accepted by the peer
//    QObject::connect(_wallet, &bitcoin::SPVWallet::broadcastFailed, [this](Coin::TransactionId txid) {
//        if(_transactionsWaitingNotification.find(txid) != _transactionsWaitingNotification.end()) {

//            if(_transactionsWaitingNotification[txid].error)
//            _transactionsWaitingNotification[txid].error(std::runtime_error("transaction broadcast failed"));

//            _transactionsWaitingNotification.erase(txid);
//        }
//    });
}

size_t TransactionSendQueue::size() const {
    return _queuedTransactions.size();
}

void TransactionSendQueue::flush() {
    if(size() == 0)
        return;

    auto now = std::chrono::high_resolution_clock::now();

    if((now - _lastFlush) < _minFlushInterval)
        return;

    if(_wallet->isConnected()) {
        _lastFlush = now;

        for(auto &entry : _queuedTransactions) {

            std::cout << "Broadcasting Transaction: " << entry.first.toRPCByteOrder() << std::endl;
            _transactionsWaitingNotification[entry.first] = entry.second;

            _wallet->broadcastTx(entry.second.transaction);
        }

        _queuedTransactions.clear();
    }

}

void TransactionSendQueue::insert(const Coin::Transaction &tx,
                                  const TransactionBroadcastSuccessCallback onSuccess,
                                  const TransactionBroadcastErrorCallback onError) {
    _queuedTransactions[Coin::TransactionId::fromTx(tx)] = TransactionAndCallback({tx, onSuccess, onError});
    flush();
}

}
}
