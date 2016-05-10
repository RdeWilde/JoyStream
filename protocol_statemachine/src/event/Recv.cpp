/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 10 2016
 */

#include <protocol_statemachine/event/Recv.hpp>

namespace joystream {
namespace protocol_statemachine {
namespace event {

    template <class T>
    Recv<T>::Recv() {
    }

    template <class T>
    Recv<T>::Recv(const T & message)
        : _message(message) {
    }

    template <class T>
    T Recv<T>::message() const {
        return _message;
    }

    template <class T>
    void Recv<T>::setMessage(const T & message) {
        _message = message;
    }

}
}
}
