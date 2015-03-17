#ifndef PRIVATE_KEY_HPP
#define PRIVATE_KEY_HPP

#include <QtGlobal>

#include <vector>

class QDataStream;
class QString;

class PrivateKey
{
public:

    // Byte length of raw data
    static const quint32 length = 32;

    // Construct from base58 encoded string
    // e.g. 4b13890d16944ab1d8dd1ef4ac604fe589c62c27af74fc7250c4d01e2a1915d9
    PrivateKey(const QString & string);

    // Default/Copy constructor and assignment operator needed to put in container.
    PrivateKey();
    PrivateKey(const PrivateKey & o);
    PrivateKey & operator=(const PrivateKey & o);

    // Utilities
    void clear();
    bool isClear() const;

    // Encodes as base58
    QString toString() const;

    // Stream operator
    friend QDataStream & operator<<(QDataStream & stream, const PrivateKey & o);
    friend QDataStream & operator>>(QDataStream & stream, PrivateKey & o);

    // Comparison for use with QMap
    bool operator<(const PrivateKey & o) const;
    bool operator==(const PrivateKey & o) const;

    std::vector<unsigned char> buffer() const;
    void setBuffer(const std::vector<unsigned char> & buffer);

private:

    // Raw data
    // 0 is most significant byte for comparisons
    std::vector<unsigned char> _buffer;
};

uint qHash(const PrivateKey & o);

#endif // PRIVATE_KEY_HPP
