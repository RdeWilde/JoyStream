
#ifndef WALLET_UTILITIES_HPP
#define WALLET_UTILITIES_HPP

class QByteArray;
class QDateTime;

namespace Wallet {
namespace Utilities {

    QByteArray encodeDateTime(const QDateTime & dateTime);
    QDateTime decodeDateTime(const QByteArray & encodedDateTime);

}
}

#endif // WALLET_UTILITIES_HPP
