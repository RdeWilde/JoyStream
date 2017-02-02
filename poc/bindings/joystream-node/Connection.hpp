/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 2 2017
 */

#ifndef JOYSTREAM_NODE_CONNECTION_HPP
#define JOYSTREAM_NODE_CONNECTION_HPP

#include <nan.h>
#include <protocol_session/Status.hpp>
#include <libtorrent/socket_io.hpp>

namespace joystream {
namespace node {
namespace connection {

 /* @brief Creates javascript representation of inner (i.e. simple)
  * states in protocol_statemachine::CBStateMachine.

  * @param i type_index object for state class
  * @return v8::Local<v8::Uint32> where value the value for state protoco_statemachine::X is the same
  * as what is found by inspecting InnerStateType.X.
  */
  v8::Local<v8::Uint32> encode(const std::type_index & i);
  v8::Local<v8::Object> encode(const joystream::protocol_session::status::Connection<libtorrent::tcp::endpoint>& c);

}
}
}

#endif // JOYSTREAM_NODE_CONNECTION_HPP
