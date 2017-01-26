/**
* Copyright (C) JoyStream - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Bedeho Mender <bedeho.mender@gmail.com>, January 19 2017
*/

#include "StartDownloadConnectionInformation.hpp"
#include "SellerTerms.hpp"
#include "PrivateKey.hpp"
#include "PubKeyHash.hpp"
#include "utils.hpp"

#include <protocol_session/protocol_session.hpp>

#define SELLER_TERMS_KEY "sellerTerms"
#define INDEX_KEY "index"
#define VALUE_KEY "value"
#define BUYER_CONTRACT_SK_KEY "buyerContractSk"
#define BUYER_FINAL_PK_HASH_KEY "buyerFinalPkHash"

namespace joystream {
namespace node {
namespace StartDownloadConnectionInformation {

NAN_MODULE_INIT(Init) {

}

v8::Local<v8::Object> encode(const protocol_session::StartDownloadConnectionInformation & inf) {

  v8::Local<v8::Object> o = Nan::New<v8::Object>();

  SET_VAL(o, SELLER_TERMS_KEY, SellerTerms::toObject(inf.sellerTerms));
  SET_UINT32(o, INDEX_KEY, inf.index);
  SET_NUMBER(o, VALUE_KEY, inf.value);
  SET_VAL(o, BUYER_CONTRACT_SK_KEY, PrivateKey::toValue(inf.buyerContractKeyPair.sk()));
  SET_VAL(o, BUYER_FINAL_PK_HASH_KEY, PubKeyHash::toValue(inf.buyerFinalPkHash));

  return o;
}

protocol_session::StartDownloadConnectionInformation decode(const v8::Local<v8::Object> & o) {

  SellerTerms terms(SellerTerms::fromObject(GET_VAL(o, SELLER_TERMS_KEY)));
  PrivateKey sk(PrivateKey::fromValeu());
  PubKeyHash pubKeyHash(PubKeyHash::fromValue(GET_VAL(o, BUYER_FINAL_PK_HASH_KEY)));

  return protocol_session::StartDownloadConnectionInformation(terms,
                                                              GET_UINT32(o, INDEX_KEY),
                                                              GET_INT64(o, VALUE_KEY),
                                                              Coin::KeyPair(sk, sk.toPublic()),
                                                              pubKeyHash);
}

}
}
}
