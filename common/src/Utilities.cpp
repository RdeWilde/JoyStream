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

    uint32_t popData(const uchar_vector & inputData, uchar_vector & poppedData)
    {
        if(inputData.empty() || inputData[0] == 0) return 0;

        uint32_t inputSize = inputData.size();
        uint32_t dataSize = 0;
        uint32_t dataStartsAt = 0;
        uint32_t dataEndsAt = 0;

        if (inputData[0] <= 0x4b  && inputSize > 1) {
            dataSize = inputData[0];
            dataStartsAt = 1;
        }
        else if (inputData[0] == 0x4c  && inputSize > 2) {
            dataSize = inputData[1];
            dataStartsAt = 2;
        }
        else if (inputData[0] == 0x4d  && inputSize > 3) {
            dataSize = inputData[1];
            dataSize += (inputData[2] << 8);
            dataStartsAt = 3;
        }
        else if(inputData[0] == 0x4e  && inputSize > 5){
            dataSize = inputData[1];
            dataSize += (inputData[2] << 8);
            dataSize += (inputData[3] << 16);
            dataSize += (inputData[4] << 24);
            dataStartsAt = 5;
        }

        if(dataSize > 0) {
            dataEndsAt = dataStartsAt + dataSize;

            if(dataEndsAt <= inputSize) {
                poppedData = uchar_vector(&inputData[dataStartsAt], &inputData[dataEndsAt]);
                return dataEndsAt;
            }
        }

        return 0;
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

    uchar_vector sighash(const Coin::Transaction & tx,
                    const typesafeOutPoint &outPoint,
                    const uchar_vector & subscript,
                    const SigHashType & sigHashType) {

        // We only support this for now
        if(sigHashType.type() != SigHashType::MutuallyExclusiveType::all)
            throw std::runtime_error("unsupported sighash type, only sighash_all is supported");

        // Make a copy of the transaction
        Coin::Transaction txCopy = tx;

        // Sign only the input which spends outPoint
        if(sigHashType.anyOneCanPay()) {
            // https://en.bitcoin.it/wiki/OP_CHECKSIG#Procedure_for_Hashtype_SIGHASH_ANYONECANPAY
            // Remove all inputs
            txCopy.inputs.clear();

            for(auto & txinput : tx.inputs) {
                if(typesafeOutPoint(txinput.previousOut) == outPoint) {
                    // This is the input to sign
                    txCopy.inputs.push_back(txinput);
                    txCopy.inputs[0].scriptSig = subscript;
                    break;
                }
            }

            if(txCopy.inputs.size() != 1) {
                throw std::runtime_error("Transaction does not have a corresponding input");
            }

        } else {  // Sign all inputs

            bool foundInput = false;

            // Clear input signatures
            for(auto & txinput : txCopy.inputs) {
                if(typesafeOutPoint(txinput.previousOut) == outPoint) {
                    txinput.scriptSig = subscript;
                    foundInput = true;
                } else {
                    txinput.scriptSig.clear();
                }
            }

            if(!foundInput) {
                throw std::runtime_error("Transaction does not have a corresponding input");
            }
        }

        // Compute sighash
        return txCopy.getHashWithAppendedCode(sigHashType.hashCode());
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
