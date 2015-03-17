#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include <QtGlobal>

#include <vector>

class QDataStream;
class QString;

class PublicKey
{
public:

    // Byte length of raw data
    static const quint32 length = 33;

    // Construct from base58 encoded string
    // e.g. 02520b32e461ec9e6b685d0e8603cc9b345bda4624aeb753a02333340d66909c83
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

    std::vector<unsigned char> buffer() const;
    void setBuffer(const std::vector<unsigned char> & buffer);

private:

    // Raw data
    // 0 is most significant byte for comparisons
    std::vector<unsigned char> _buffer;
};

uint qHash(const PublicKey & o);

#endif // PUBLIC_KEY_HPP
