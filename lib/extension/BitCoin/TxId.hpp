#ifndef TX_ID_HPP
#define TX_ID_HPP

#include <QtGlobal> // quint32, ...

class QDataStream;

class TxId
{
public:

    // Byte length of raw data
    static const quint32 length = 32;

    // Defualt constructor
    TxId();

    // Copy constructor
    TxId(const TxId & o);

    // Construct from hex encoded string, hence length of string must be 2*length
    // e.g. 71d19ec329ece037de94a77dfcb9e2d1a3738e1f364e953adc82048e1f567d67
    TxId(const QString & string);

    // Asignemtn operator
    TxId & operator=(const TxId & o);

    // Utilities
    char at(unsigned int index) const;
    void set(unsigned int index, char value);
    void clear();
    bool isClear() const;
    const char * buffer() const;
    char * buffer();

    // Encodes as hex string of length 2*length
    QString toString() const;

    // Stream operator
    friend QDataStream & operator<<(QDataStream & stream, const TxId & o);
    friend QDataStream & operator>>(QDataStream & stream, TxId & o);

    // Comparison for use with QMap
    bool operator<(const TxId & o) const;
    bool operator==(const TxId & o) const;

private:

    // Actual type fixed buffer
    // 0 is most significant byte for comparisons
    // Raw encoding
    char _buffer[length];
};

#include <QMetaType>
Q_DECLARE_METATYPE(TxId)

uint qHash(const TxId & o);

#endif // TX_ID_HPP
