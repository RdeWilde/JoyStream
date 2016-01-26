#include <blockcypher/TX.hpp>
#include <QDateTime>
#include <QDebug>

namespace BlockCypher {

    TX::TX() {

    }

    TX::TX(const QJsonObject & o) {
        throw std::runtime_error("not implemented yet");
    }

}
