/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/Utilities.hpp>

#include <stdutils/uchar_vector.h>
#include <common/Network.hpp>

#include <QByteArray>

namespace Coin {

    QByteArray toByteArray(const uchar_vector & raw) {

        // Get pointer to data
        const char * data = (const char *)raw.data();

        // Construct byte array and return it
        return QByteArray(data, raw.size());
    }

    uchar_vector toUCharVector(const QByteArray & array) {

        // Get pointer to data
        const unsigned char * data = (const unsigned char *)array.data();

        // Construct uchar_vector
        return uchar_vector(data, array.size());

    }

    const unsigned char * networkToAddressVersions(Network network) {

        switch(network) {
            case Network::testnet3: return testnet3AddressVersions;
            case Network::mainnet: return mainnetAddressVersions;
            default:
                    Q_ASSERT(false);
        }
    }

}

