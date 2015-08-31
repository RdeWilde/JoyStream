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
    */

#include <CoinCore/secp256k1.h>

    bytes_t sighash(const Coin::Transaction & tx, uint input, const CoinQ::Script::Script & inputScriptBuilder) {

        // Make copy of original tx, since it will be modified
        Coin::Transaction txCopy = tx;

        // Clear all input scripts
        for(std::vector<TxIn>::iterator i = txCopy.inputs.begin(),
            end = txCopy.inputs.end(); i != end;i++)
            (*i).scriptSig.clear();

        // Set script sig for input to be signed
       txCopy.inputs[input].scriptSig = inputScriptBuilder.txinscript(CoinQ::Script::Script::sigtype_t::SIGN);

       // Compute sighash
       bytes_t signingHash = txCopy.getHashWithAppendedCode(CoinQ::Script::SigHashType::SIGHASH_ALL);

       return signingHash;
    }

     secure_bytes_t createSignature(const Coin::Transaction & tx,
                                    uint inputToSign,
                                    const CoinQ::Script::Script & inputScriptBuilder,
                                    const uchar_vector & privateKey) {

        // Generate sighash
        bytes_t signingHash = sighash(tx, inputToSign, inputScriptBuilder);

        // Create signing key
        CoinCrypto::secp256k1_key signingKey;
        signingKey.setPrivKey(privateKey);

        // Comute signature and return
        return CoinCrypto::secp256k1_sign(signingKey, signingHash);
     }

     bool verifySignature(const Coin::Transaction & tx,
                          uint inputToCheck,
                          const CoinQ::Script::Script & inputScriptBuilder,
                          const secure_bytes_t & signature,
                          const bytes_t & publicKey) {

         // Generate sighash
         bytes_t signingHash = sighash(tx, inputToCheck, inputScriptBuilder);

         // Create signature checking key
         CoinCrypto::secp256k1_key signatureCheckingKey;
         signatureCheckingKey.setPubKey(publicKey);

         // Check signature and return
         return CoinCrypto::secp256k1_verify(signatureCheckingKey, signingHash, signature);
     }
}
