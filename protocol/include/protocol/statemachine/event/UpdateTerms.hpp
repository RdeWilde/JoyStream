/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATEMACHINE_UPDATETERMS_HPP
#define JOYSTREAM_PROTOCOL_STATEMACHINE_UPDATETERMS_HPP

#include <boost/statechart/event.hpp>
#include <protocol/SellerTerms.hpp>

namespace sc = boost::statechart;

namespace joystream {
namespace protocol {
namespace statemachine {
namespace event {

    template <class T>
    class UpdateTerms : public sc::event<UpdateTerms<T>> {

    public:

        UpdateTerms(const T & );

        // Getters and setters
        T terms() const;
        void setTerms(const T &);

    private:

        T _terms;
    };

}
}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol/../../src/statemachine/event/UpdateTerms.cpp>


#endif // JOYSTREAM_PROTOCOL_STATEMACHINE_UPDATETERMS_HPP
