#include "Wallet.hpp"

/**
 * Wallet::Entry
 */
Wallet::Entry::Entry(const OutPoint & point, Type type, const QString & description, const QDateTime & created, bool spent, const Hash & blockHash, quint32 blockHeight)
    : _point(point)
    , _type(type)
    , _description(description)
    , _created(created)
    , _spent(spent)
    , _blockHash(blockHash)
    , _blockHeight(blockHeight){
}

OutPoint Wallet::Entry::point() const {
    return _point;
}

void Wallet::Entry::setPoint(const OutPoint &point) {
    _point = point;
}

Wallet::Entry::Type Wallet::Entry::type() const
{
    return _type;
}

void Wallet::Entry::setType(const Type & type) {
    _type = type;
}

QString Wallet::Entry::description() const
{
    return _description;
}

void Wallet::Entry::setDescription(const QString & description) {
    _description = description;
}

QDateTime Wallet::Entry::created() const {
    return _created;
}

void Wallet::Entry::setCreated(const QDateTime & created) {
    _created = created;
}

bool Wallet::Entry::spent() const {
    return _spent;
}

void Wallet::Entry::setSpent(bool spent) {
    _spent = spent;
}

Hash Wallet::Entry::blockHash() const {
    return _blockHash;
}

void Wallet::Entry::setBlockHash(const Hash & blockHash) {
    _blockHash = blockHash;
}

quint32 Wallet::Entry::blockHeight() const {
    return _blockHeight;
}

void Wallet::Entry::setBlockHeight(quint32 blockHeight) {
    _blockHeight = blockHeight;
}

/**
 * Wallet
 */

quint64 Wallet::balance(quint32 confirmations) const {
    return 0;
}
