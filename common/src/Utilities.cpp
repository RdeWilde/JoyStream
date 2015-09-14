/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/Utilities.hpp>

//#include <CoinCore/secp256k1.h>
#include <stdutils/uchar_vector.h>
#include <common/Network.hpp>
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <common/P2PKHScriptSig.hpp>

#include <QByteArray>
#include <QDebug>

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

    uchar_vector opPushData(uint32_t nBytes)
    {
        uchar_vector rval;
        if (nBytes <= 0x4b) {
            rval.push_back((unsigned char)nBytes);
        }
        else if (nBytes <= 0xff) {
            rval.push_back(0x4c);
            rval.push_back((unsigned char)nBytes);
        }
        else if (nBytes <= 0xffff) {
            rval.push_back(0x4d);
            rval.push_back((unsigned char)(nBytes & 0xff));
            rval.push_back((unsigned char)(nBytes >> 8));
        }
        else {
            rval.push_back(0x4e);
            rval.push_back((unsigned char)(nBytes & 0xff));
            rval.push_back((unsigned char)((nBytes >> 8) & 0xff));
            rval.push_back((unsigned char)((nBytes >> 16) & 0xff));
            rval.push_back((unsigned char)(nBytes >> 24));
        }

        return rval;
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

    // bytes_t sighash(const Coin::Transaction & tx, uint input, const CoinQ::Script::Script & inputScriptBuilder) {
    uchar_vector sighash(const Coin::Transaction & tx,
                    uint input,
                    const uchar_vector & scriptPubKey,
                    const SigHashType & type) {

        // We only support this for now
        if(!type.isStandard())
            throw std::runtime_error("unsupported sighash type, only sighash_all is supported");

        if(tx.inputs.size() <= input)
            throw std::runtime_error("Transaction does not have a corresponding input");

        // Make copy of original tx, since it will be modified
        Coin::Transaction txCopy = tx;

        // Clear all input scripts
        for(std::vector<TxIn>::iterator i = txCopy.inputs.begin(),
            end = txCopy.inputs.end(); i != end;i++)
            (*i).scriptSig.clear();

        /**
         * // Clear all op_codeseperators from scriptPubKey
         * uchar_vector cleanScriptPubKey = clearCodeSeperators(scriptPubKey);
         */

        // Set script sig for input to be signed
        txCopy.inputs[input].scriptSig = scriptPubKey;

        // Compute sighash and return
        return txCopy.getHashWithAppendedCode(type.hashCode());
    }

    /**
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

     }
     */

    uchar_vector toScriptSigForm(const std::vector<TransactionSignature> & sigs) {

        uchar_vector serialized;

        // Add leading OP_0 bug thing
        serialized.push_back(0x00); // OP_0

        // Add each signature and corresponding sighash flag
        for(std::vector<TransactionSignature>::const_iterator i = sigs.cbegin(),
            end = sigs.cend(); i != end; i++)
            serialized += (*i).serializeForScriptSig();

        return serialized;

    }

    void setScriptSigToSpendP2PKH(Coin::Transaction & tx,
                           uint input,
                           const Coin::PrivateKey & sk) {

        /**
        qDebug() << "tx: " << QString::fromStdString(tx.getHash/LittleEndian().getHex());
        qDebug() << "input: " << input;
        qDebug() << "sk: " << sk.toHex();
        */

        // Generate signature
        Coin::TransactionSignature ts = sk.signForP2PKHSpend(tx, input);

        //qDebug() << "signature: " << ts.sig().toString();

        // Generate scriptSig
        Coin::P2PKHScriptSig scriptSig(sk.toPublicKey(), ts);

        Q_ASSERT(input < tx.inputs.size());

        uchar_vector ser_scriptSig = scriptSig.serialized();

        //qDebug() << "scriptSig:" << QString::fromStdString(ser_scriptSig.getHex());

        // Set input script
        tx.inputs[input].scriptSig = ser_scriptSig;


    }
}
