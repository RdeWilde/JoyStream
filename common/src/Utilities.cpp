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

    /**
    unsigned int extendedPrivateKeyVersionBytes(Network network) {

        switch(network) {

            case Network::testnet3: return 0x04358394;
            case Network::mainnet: return 0x0488ADE4;
            default:
                Q_ASSERT(false);
        }
    }

    unsigned int extendedPublicKeyVersionBytes(Network network) {

        switch(network) {

            case Network::testnet3: return 0x043587CF;
            case Network::mainnet: return 0x0488B21E;
            default:
                Q_ASSERT(false);
        }
    }



    unsigned int toBase58CheckVersionBytes(AddressType type, Network network) {

        switch(type) {

            case AddressType::PayToPublicKeyHash:

                if(network == Network::mainnet)
                    return 0x0; // decimal 0
                else if(network == Network::testnet3)
                    return 0x6F; // decimal 111;

                break;

            case AddressType::PayToScriptHash:

                break;

                if(network == Network::mainnet)
                    return 0x5; // decimal 5
                else if(network == Network::testnet3)
                    return 0xC4; // decimal 96

            default:
                Q_ASSERT(false);
        }
    }

    AddressType versionByteToAddressType(unsigned int version) {

        switch(version) {

            case 0x0: return AddressType::PayToPublicKeyHash;
            case 0x6F: return AddressType::PayToPublicKeyHash;
            case 0x5: return AddressType::PayToScriptHash;
            case 0xC4: return AddressType::PayToScriptHash;
            default:
                Q_ASSERT(false);
        }
    }

    Network versionByteToNetwork(unsigned int version) {

        switch(version) {

            case 0x0: return Network::mainnet;
            case 0x6F: return Network::testnet3;
            case 0x5: return Network::mainnet;
            case 0xC4: return Network::testnet3;
            default:
                Q_ASSERT(false);
        }
    }


    Network getNetwork(std::string & base58CheckEncodedAddress) {

        // do checks and testing

        // throw exception if ntohing matches
    }

    AddressType getType(std::string & base58CheckEncodedAddress) {

        // do checks and testing

        // throw exception if ntohing matches
    }
    */

    /**
    QByteArray uchar_vector_to_QByteArray(const uchar_vector & v) {

        // Reinterpret from unsigned to signed char
        const char * data = reinterpret_cast<const char *>(v.data());

        // Create
        return QByteArray(data, v.size());
    }
    */

}

