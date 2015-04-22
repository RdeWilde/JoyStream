#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include <QtGlobal>

#include <vector>

class QDataStream;
class QString;

//
class PublicKey
{
public:

    // Byte length of raw data
    static const quint32 length = 33;

    // Construct from hex encoded string of DER encoded compressed public key
    PublicKey(const QString & string);

    // Default/Copy constructor and assignment operator needed to put in container.
    PublicKey();
    PublicKey(const PublicKey & o);
    PublicKey & operator=(const PublicKey & o);

    // Utilities
    void clear();
    bool isClear() const;

    // Encodes as base58
    QString toString() const;

    // Stream operator
    friend QDataStream & operator<<(QDataStream & stream, const PublicKey & o);
    friend QDataStream & operator>>(QDataStream & stream, PublicKey & o);

    // Comparison for use with QMap
    bool operator<(const PublicKey & o) const;
    bool operator==(const PublicKey & o) const;

    std::vector<char> buffer() const;
    void setBuffer(const std::vector<char> & buffer);

private:

    // DER encoded compressed public key
    // 0 is most significant byte for comparisons
    std::vector<char> _buffer;
};

uint qHash(const PublicKey & o);

#endif // PUBLIC_KEY_HPP
