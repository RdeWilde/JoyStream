#include <app_kit/TransactionSendQueue.hpp>

namespace joystream {
namespace appkit {

TransactionSendBuffer::TransactionSendBuffer(bitcoin::SPVWallet *wallet)
    : _wallet(wallet) {
}

size_t TransactionSendBuffer::size() const {
    return _transactions.size();
}

void TransactionSendBuffer::flush() {
    if(size() == 0)
        return;

    if(_wallet->isConnected()) {

        for(auto &entry : _transactions)
        {
            std::cout << "Broadcasting Transaction: " << entry.first.toRPCByteOrder() << std::endl;
            _wallet->broadcastTx(entry.second);
        }

        _transactions.clear();
    }
}

void TransactionSendBuffer::insert(const Coin::Transaction &tx) {
    _transactions[Coin::TransactionId::fromTx(tx)] = tx;
    flush();
}

}
}
