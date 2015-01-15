#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include <QtGlobal> // quint32
class QDataStream;

#define PUBLIC_KEY_LENGTH 3

class PublicKey
{
public:
    PublicKey();

    static const quint32 length = PUBLIC_KEY_LENGTH;

    QString wif() const;

    friend QDataStream & operator<<(QDataStream & stream, const PublicKey & key);
    friend QDataStream & operator>>(QDataStream & stream, PublicKey & key);

private:

    char _raw[PUBLIC_KEY_LENGTH];
};

#endif // PUBLIC_KEY_HPP
