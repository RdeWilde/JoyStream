#ifndef HASH_HPP
#define HASH_HPP

class QDataStream;

#include <QtGlobal> // quint32;

class Hash
{
public:
    Hash();

    static const quint32 length = 32;

    friend QDataStream & operator<<(QDataStream & stream, const Hash & key);
    friend QDataStream & operator>>(QDataStream & stream, Hash & key);

private:

    char _raw[32];
};

#endif // HASH_HPP
