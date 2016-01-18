#include <blockcypher/TX.hpp>
#include <QDateTime>
#include <QDebug>


namespace BlockCypher {

    TX::TX() { }

    // S E T  M E T H O D S
    void TX::setHash(QString const & hash) {
        _hash = hash;
        qDebug() << "\n Inserted _hash:" << _hash;
    }

    void TX::setBlockHeight(const qint32 &blockheight) {
        //Could be -1 at 0 conf, therefore not unsigned. 
        _block_height = blockheight;
        qDebug() << "\n Inserted _block_height:" << _block_height;
    }

    void TX::setAddresses(const QStringList &addresses) {
        _addresses = addresses;
        qDebug() << "\n Inserted _addresses:" << _addresses;
    }

    void TX::setTotal(const quint64 &total) {
        _total = total;
        qDebug() << "\n Inserted _total:" << _total;
    }

    void TX::setFees(const quint64 &fees) {
        _fees = fees;
        qDebug() << "\n Inserted _fees:" << _fees;

    }

    void TX::setReceived(const QDateTime &received) {
        _received = received;
        qDebug() << "\n Inserted _received:" << _received.toString();
    }

    void TX::setVersion(const uint32_t &version) {
        _ver = version;
        qDebug() << "\n Inserted _ver:" << _ver;
    }

    void TX::setLockTime(const uint32_t &locktime) {
        _lock_time = locktime;
        qDebug() << "\n Inserted _lock_time:" << _lock_time;
    }

    void TX::setDoubleSpend(const bool &doublespend) {
        _double_spend = doublespend;
        qDebug() << "\n Inserted _double_spend:" << _double_spend;
    }

    void TX::setNumInputs(const int &numinputs) {
        _vin_sz = numinputs;
        qDebug() << "\n Inserted _vin_sz:" << _vin_sz;
    }

    void TX::setNumOutputs(const int &numoutputs) {
        _vout_sz = numoutputs;
        qDebug() << "\n Inserted _vout_sz:" << _vout_sz;
    }

    void TX::setConfirmations(int const & confirmations) {
        _confirmations = confirmations;
        qDebug() << "\n Inserted confirmations:" << confirmations;
    }

    void TX::setInputs(const QList<TXInput> &inputs) {
        _inputs = inputs;
    }

    void TX::setOutputs(const QList<TXOutput> &outputs) {
        _outputs = outputs;
    }

    void TX::setConfirmed(const QDateTime &confirmed) {
        //Not tested - needs to have at least 1 conf
        _confirmed = confirmed;
        qDebug() << "\n Inserted _confirmed: " << _confirmed;
    }

    void TX::setBlockHash(const QString &blockhash) {
        //Not tested - needs to have at least 1 conf
        _block_hash = blockhash;
    }

    // G E T  M E T H O D S

    QString TX::getHash(void) const { return _hash; }
    qint32 TX::getBlockHeight(void) const { return _block_height; }
    QStringList TX::getAddresses(void) const { return _addresses; }
    quint64 TX::getTotal(void) const { return _total; }
    quint64 TX::getFees(void) const { return _fees; }
    QDateTime TX::getReceived(void) const { return _received; }
    uint32_t TX::getVersion(void) const { return _ver; }
    uint32_t TX::getLockTime(void) const { return _lock_time; }
    bool TX::getDoubleSpend(void) const { return _double_spend; }
    int TX::getNumInputs(void) const { return _vin_sz; }
    int TX::getNumOutputs(void) const { return _vout_sz; }
    int TX::getConfirmations(void) const { return _confirmations; }
    QList<TXInput> TX::getInputs(void) const { return _inputs; }
    QList<TXOutput> TX::getOutputs(void) const { return _outputs; }
    QDateTime TX::getConfirmed(void) const { return _confirmed; }
    Coin::BlockId TX::getBlockHash(void) const { return _block_hash; }

} // end namespace BlockCypher
