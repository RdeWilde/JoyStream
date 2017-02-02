/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 2 2017
 */

#include "Connection.hpp"
#include "libtorrent-node/utils.hpp"

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
  v8::Local<v8::Object> encode(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c) {

  }

}
}
}
