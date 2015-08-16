
#ifndef WALLET_UTILITIES_HPP
#define WALLET_UTILITIES_HPP

class QByteArray;
class QDateTime;

class WalletUtilities {

public:

    static QByteArray encodeDateTime(const QDateTime & dateTime);
    static QDateTime decodeDateTime(const QByteArray & encodedDateTime);

};

#endif // WALLET_UTILITIES_HPP
