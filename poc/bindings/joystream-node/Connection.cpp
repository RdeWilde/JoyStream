/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 2 2017
 */

#include "Connection.hpp"
#include "libtorrent-node/utils.hpp"
#include "SellerTerms.hpp"
#include "BuyerTerms.hpp"

namespace joystream {
namespace node {
namespace connection {

  typedef std::pair<std::type_index, const char *> TypeInfo;
  #define STATE_TO_TYPE_INFO(name) std::make_pair(std::type_index(typeid(protocol_statemachine::name)), #name)

  static const std::array<TypeInfo, 14> InnerStateTypeInfo = {

      // Initial picker state, in practice we should never be here given
      // current behaviour of plugin
      STATE_TO_TYPE_INFO(ChooseMode),

      // * Observing
      STATE_TO_TYPE_INFO(Observing),

      // * Selling
      STATE_TO_TYPE_INFO(ReadyForInvitation),
      STATE_TO_TYPE_INFO(Invited),
      // (active selling state)
      STATE_TO_TYPE_INFO(WaitingToStart),
      STATE_TO_TYPE_INFO(ReadyForPieceRequest),
      // ** Servicing Piece Request
      STATE_TO_TYPE_INFO(LoadingPiece),
      STATE_TO_TYPE_INFO(WaitingForPayment),

      // * Buying
      STATE_TO_TYPE_INFO(ReadyToInviteSeller),
      STATE_TO_TYPE_INFO(WaitingForSellerToJoin),
      // ** Seller has joined
      STATE_TO_TYPE_INFO(PreparingContract),
      // (active buying state)
      STATE_TO_TYPE_INFO(ReadyToRequestPiece),
      STATE_TO_TYPE_INFO(WaitingForFullPiece),
      STATE_TO_TYPE_INFO(ProcessingPiece)
  };

  NAN_MODULE_INIT(InitInnerStateTypes);
  NAN_MODULE_INIT(InitInnerStateTypes) {

    v8::Local<v8::Object> o = Nan::New<v8::Object>();

    for(int i = 0;i < InnerStateTypeInfo.size();i++)
      SET_NUMBER(o, InnerStateTypeInfo[i].second, i);


    SET_VAL(target, "InnerStateType", o);
  }

  v8::Local<v8::Uint32> encode(const std::type_index & index){

    for(int i = 0;i < InnerStateTypeInfo.size();i++) {

      if(InnerStateTypeInfo[i].first == index)
        return Nan::New<v8::Uint32>(i);
    }

    // Should never get here, means our code is out of synch
    // with structure of statemachine. We throw exception to detect, since
    // bindings never used in debug mode. This *ugly* code is the price of no types :/
    throw std::runtime_error("out of synch with state machine, bad build!");
  }

  v8::Local<v8::Object> encode(const protocol_statemachine::AnnouncedModeAndTerms & a) {

    v8::Local<v8::Object> o = Nan::New<v8::Object>();

    switch(a.modeAnnounced()) {
      case protocol_statemachine::ModeAnnounced::none:

        SET_BOOL(o, "none", true);
        break;

      case protocol_statemachine::ModeAnnounced::observe:
      
        SET_BOOL(o, "observer", true);
        break;

      case protocol_statemachine::ModeAnnounced::sell:

        {
          v8::Local<v8::Object> s = Nan::New<v8::Object>();
          SET_VAL(s, "terms", seller_terms::encode(a.sellModeTerms()));
          SET_NUMBER(s, "index", a.index());

          SET_VAL(o, "seller", s);
        }

        break;

      case protocol_statemachine::ModeAnnounced::buy:

        {

          v8::Local<v8::Object> s = Nan::New<v8::Object>();
          SET_VAL(s, "terms", buyer_terms::encode(a.buyModeTerms()));

          SET_VAL(o, "buyer", s);
        }

        break;
    }

    return o;
  }

  v8::Local<v8::Object> encode(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c) {

  }

}
}
}
