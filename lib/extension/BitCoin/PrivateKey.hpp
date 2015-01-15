#ifndef PRIVATE_KEY_HPP
#define PRIVATE_KEY_HPP

#include <QtGlobal> // quint32
class QDataStream;

#define PRIVATE_KEY_LENGTH 7

class PrivateKey
{
public:
    PrivateKey();

    static const quint32 length = PRIVATE_KEY_LENGTH;

    friend QDataStream & operator<<(QDataStream & stream, const PrivateKey & key);
    friend QDataStream & operator>>(QDataStream & stream, PrivateKey & key);

private:

    char _raw[PRIVATE_KEY_LENGTH];
};

#endif // PRIVATE_KEY_HPP
