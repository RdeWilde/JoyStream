#ifndef HASH_HPP
#define HASH_HPP

class QDataStream;

#include <QtGlobal> // quint32;

#define HASH_LENGTH 32

class Hash
{
public:
    Hash();

    static quint32 length();

    friend QDataStream & operator<<(QDataStream & stream, const Hash & key);
    friend QDataStream & operator>>(QDataStream & stream, Hash & key);

private:

    char _hash[HASH_LENGTH];
};

#endif // HASH_HPP
