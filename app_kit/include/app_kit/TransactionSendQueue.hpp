#ifndef JOYSTREAM_APPKIT_TRANSACTION_SEND_QUEUE_HPP
#define JOYSTREAM_APPKIT_TRANSACTION_SEND_QUEUE_HPP

#include <bitcoin/SPVWallet.hpp>

namespace joystream {
namespace appkit {

class TransactionSendQueue
{
    typedef std::chrono::duration<double> Interval;
public:
    TransactionSendQueue(bitcoin::SPVWallet*, const Interval = Interval(20));

    size_t size() const;

    void flush();

    void insert(const Coin::Transaction &);

private:
    bitcoin::SPVWallet* _wallet;
    std::map<Coin::TransactionId, Coin::Transaction> _transactions;
    std::chrono::duration<double> _minFlushInterval;
    std::chrono::high_resolution_clock::time_point _lastFlush;
};

}
}
#endif // JOYSTREAM_APPKIT_TRANSACTION_SEND_QUEUE_HPP
