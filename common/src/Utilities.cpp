/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <common/Utilities.hpp>

#include <stdutils/uchar_vector.h>
#include <common/Network.hpp>
#include <common/SigHashType.hpp>
#include <common/TransactionSignature.hpp>
#include <common/PrivateKey.hpp>
#include <common/P2PKHScriptSig.hpp>
#include <common/typesafeOutPoint.hpp>

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
            case Network::regtest: return testnet3AddressVersions;
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

    /*
     * This method extracts and returns the next data item from the script
     * and next is set to point to the next byte after the data
     *
     * If the first byte (operation) is not a push data operation or
     * script is too short to contain the expected data the return value will be an empty uchar_vector
     * and next will point to the end() of the script
     */
    uchar_vector popData(uchar_vector & script, uchar_vector::iterator & next)
    {
        next = script.end();

        if(script.empty()) return uchar_vector();

        uint32_t dataSize = 0;
        uint32_t offset = 0;

        if (script[0] <= 0x4b) {
            dataSize = script[0];
            offset = 1;
        }
        else if (script[0] == 0x4c) {
            dataSize = script[1];
            offset = 1;
        }
        else if (script[0] == 0x4d) {
            dataSize = script[1];
            dataSize += (script[2] << 8);
            offset = 3;
        }
        else if(script[0] == 0x4e){
            dataSize = script[1];
            dataSize += ((uint32_t)script[2]) << 8;
            dataSize += ((uint32_t)script[3]) << 16;
            dataSize += ((uint32_t)script[4]) << 24;
            offset = 5;
        } else {
            // operation is not a push data op
        }

        if(script.size() >= (offset + dataSize)){

            if(script.size() > (offset + dataSize)){
                next = script.begin() + offset + dataSize;
            }

            if(dataSize > 0) {
                return uchar_vector(script.begin() + offset, script.begin() + offset + dataSize);
            }
        }

        return uchar_vector();
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

    uchar_vector sighash(const Transaction &tx,
                         uint inputIndex,
                         const uchar_vector &subscript,
                         const SigHashType &sigHashType) {

        // We only support this for now
        if(sigHashType.type() != SigHashType::MutuallyExclusiveType::all)
            throw std::runtime_error("unsupported sighash type, only sighash_all is supported");

        if(inputIndex >= tx.inputs.size()) {
            throw std::runtime_error("sighash: input index out of range");
        }

        // Make a copy of the transaction
        Coin::Transaction txCopy = tx;

        if(sigHashType.anyOneCanPay()) {
            // Sign only one input
            // https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_ANYONECANPAY

            // Remove all inputs
            txCopy.inputs.clear();

            // Add input to sign as input 0
            txCopy.inputs.push_back(tx.inputs[inputIndex]);

            // Set script sig to subscript for signature
            txCopy.inputs[0].scriptSig = subscript;

        } else {
            // Sign all inputs

            // Clear all inputs scripts
            txCopy.clearScriptSigs();

            // Set script sig to subscript for signature
            txCopy.inputs[inputIndex].scriptSig = subscript;
        }

        // Compute sighash
        return txCopy.getHashWithAppendedCode(sigHashType.hashCode());
    }

    uchar_vector sighash(const Coin::Transaction & tx,
                    const typesafeOutPoint &outPoint,
                    const uchar_vector & subscript,
                    const SigHashType & sigHashType) {

        // find input index by outpoint
        for(uint n = 0; n < tx.inputs.size(); n++) {
            if(typesafeOutPoint(tx.inputs[n].previousOut) == outPoint) {
                return sighash(tx, n, subscript, sigHashType);
            }
        }

        throw std::runtime_error("sighash: transaction does not have a corresponding input");
    }

    uchar_vector serializeForOP_CHECKSIGMULTISIG(const std::vector<TransactionSignature> & sigs) {

        uchar_vector serialized;

        // Test if there are any signatures at all before
        // adding leading OP_0
        if(sigs.size() > 0) {

            // Add leading OP_0 bug thing
            serialized.push_back(0x00); // OP_0

            // Add each signature and corresponding sighash flag
            for(std::vector<TransactionSignature>::const_iterator i = sigs.cbegin(),
                end = sigs.cend(); i != end; i++)
                serialized += (*i).opPushForScriptSigSerialized();

        } else
            Q_ASSERT(false);

        return serialized;

    }

    // Borrowed from bitcoin core Script.h CScript::serialize
    uchar_vector serializeScriptNum(const int64_t& value) {
       if(value == 0)
           return uchar_vector();

       uchar_vector result;
       const bool neg = value < 0;
       uint64_t absvalue = neg ? -value : value;

       while(absvalue)
       {
           result.push_back(absvalue & 0xff);
           absvalue >>= 8;
       }

//    - If the most significant byte is >= 0x80 and the value is positive, push a
//    new zero-byte to make the significant byte < 0x80 again.

//    - If the most significant byte is >= 0x80 and the value is negative, push a
//    new 0x80 byte that will be popped off when converting to an integral.

//    - If the most significant byte is < 0x80 and the value is negative, add
//    0x80 to it, since it will be subtracted and interpreted as a negative when
//    converting to an integral.

       if (result.back() & 0x80)
           result.push_back(neg ? 0x80 : 0);
       else if (neg)
           result.back() |= 0x80;

       return result;
    }

    // Borrowed from bitcoin core Script.h CScript::set_vch
    int64_t deserializeScriptNum(const uchar_vector & vch)
    {
        if (vch.empty())
         return 0;

        int64_t result = 0;
        for (size_t i = 0; i != vch.size(); ++i)
         result |= static_cast<int64_t>(vch[i]) << 8*i;

        // If the input vector's most significant byte is 0x80, remove it from
        // the result's msb and return a negative.
        if (vch.back() & 0x80)
         return -((int64_t)(result & ~(0x80ULL << (8 * (vch.size() - 1)))));

        return result;
    }
}
