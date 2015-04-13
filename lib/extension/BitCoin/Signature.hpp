#ifndef SIGNATURE_HPP
#define SIGNATURE_HPP

#include <vector>

class QDataStream;
class QString;

#define MAX_SIGNATURE_LENGTH 73

class Signature
{
public:

    // The maximum byte length of a valid signature
    const static int maxLength = 73;

    // Default/Copy constructor and assignemtn operator needed to put in container.
    Signature();
    Signature(const Signature & signature);
    Signature & operator=(const Signature& signature);

    // Hex encoded signature
    Signature(const QString & signature);

    std::vector<unsigned char> buffer() const;
    void setBuffer(const std::vector<unsigned char> & buffer);

    int length() const;

    // Hex encoded signature
    QString toString() const;

    // Stream processing
    int readFromStream(QDataStream & stream, int length);
    int writeToStream(QDataStream & stream) const;

private:

    // Raw data
    // 0 is most significant byte for comparisons
    std::vector<unsigned char> _buffer;
};

#endif // SIGNATURE_HPP
