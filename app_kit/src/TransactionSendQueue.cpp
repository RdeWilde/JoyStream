#include <app_kit/TransactionSendQueue.hpp>

namespace joystream {
namespace appkit {


TransactionSendQueue::TransactionSendQueue(bitcoin::SPVWallet *wallet, const Interval minFlushInterval)
    : _wallet(wallet),
      _minFlushInterval(minFlushInterval) {

    QObject::connect(_wallet, &bitcoin::SPVWallet::txUpdated, [this](Coin::TransactionId txid, int confirmations){
        // remove transaction from queue when it enters the wallet store
        _transactions.erase(txid);
    });
}

size_t TransactionSendQueue::size() const {
    return _transactions.size();
}

void TransactionSendQueue::flush() {
    if(size() == 0)
        return;

    auto now = std::chrono::high_resolution_clock::now();

    if((now - _lastFlush) < _minFlushInterval)
        return;

    if(_wallet->isConnected()) {
        _lastFlush = now;

        for(auto &entry : _transactions) {
            _wallet->broadcastTx(entry.second);
        }
    }

}

void TransactionSendQueue::insert(const Coin::Transaction &tx) {
    _transactions[Coin::TransactionId::fromTx(tx)] = tx;
    flush();
}

}
}
