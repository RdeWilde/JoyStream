#include "Hash.hpp"

Hash::Hash()
{
}

quint32 Hash::length() {
    return HASH_LENGTH;
}

QDataStream & Hash::operator<<(QDataStream & stream, const Hash & key) {

}

QDataStream & Hash::operator>>(QDataStream & stream, Hash & key) {

}
