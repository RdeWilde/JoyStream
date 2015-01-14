#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include <QtGlobal> // quint32
class QDataStream;

class PublicKey
{
public:
    PublicKey();

    static quint32 length();

    bool isValid() const;

    friend QDataStream & operator<<(QDataStream & stream, const PublicKey & key);
    friend QDataStream & operator>>(QDataStream & stream, PublicKey & key);
};

#endif // PUBLIC_KEY_HPP
