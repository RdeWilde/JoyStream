/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 25 2015
 */

#include <blockcypher/BlockExplorer.hpp>

#include <common/P2PKHAddress.hpp>
#include <common/TransactionId.hpp>
#include <common/Network.hpp>
#include <common/BlockId.hpp>

#define BLOCKCYPHER_EXPLORER_ROOT "https://live.blockcypher.com/"

namespace BlockCypher {
namespace BlockExplorer {

    QString urlName(Coin::Network network) {

        switch(network) {
            case Coin::Network::mainnet: return "btc";
            case Coin::Network::testnet3: return "btc-testnet";
            //case Coin::Network::bcy: return "bcy";

            default:
                Q_ASSERT(false);
        }
    }

    QString link(const Coin::P2PKHAddress & address) {
        return BLOCKCYPHER_EXPLORER_ROOT + urlName(address.network()) + "/address/" + address.toBase58CheckEncoding();
    }

    QString link(const Coin::TransactionId & txId, Coin::Network network) {
        return BLOCKCYPHER_EXPLORER_ROOT + urlName(network) + "/tx/" + txId.toHex();
    }

    QString link(const QString & name) {
        return "";
    }

    QString link(const Coin::BlockId & blockId) {
        return "";
    }

    QString link(int blockHeight) {
        return "";
    }

    }
}
