/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/TXInput.hpp>
#include <CoinCore/CoinNodeData.h> // TxIn

namespace BlockCypher {

TXInput::TXInput(const QJsonObject & o) {
    throw std::runtime_error("not done");
}

bool TXInput::operator==(const TXInput & o) {
    return _output_value == o._output_value &&
            _script == o._script;
    // no need to check last one
}

Coin::TxIn TXInput::toInput() const {
    throw std::runtime_error("not done");
    return Coin::TxIn();
}

}
