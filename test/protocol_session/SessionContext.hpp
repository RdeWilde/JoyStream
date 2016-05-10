/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#ifndef BUYINGSESSIONCONTEXT_H
#define BUYINGSESSIONCONTEXT_H

#include <protocol_session/protocol_session.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

class SessionContext {

public:

    class ConnectionEndPoint {

    public:

        // ctr (id, context)

        // connect(connectionEndpint * );
        // set _peerEndpoint = e

        // sendToPeerEndPoint(msg): give std::function<> to _context connection
        // does: _peerEndPoint->incomingMessage()

        // incomingMessage(): send function handler to session
        // does: _context->processMessage(id, m).. , but synchronized? another context thread is caller

        //

    private:

        uint _id;

        // Context in which session to which connection corresponds
        SessionContext * _context;

        // xx
        ConnectionEndPoint * _peerEndPoint;
    };

    void start() {
        // make sure we are not already started
        // start loop with sleep which
        // calls tick() on session
        // * how to process incoming messages on one of the connections?
        // * how to process the client requests to stops/pause/start?/ updateterms/remove conneciton/add
    }

    void stop() {

    }


private:

    Session<unsigned int> _session;

    //
    uint _connectionCreationsCounter;

    // map of connections to absorb
    std::map<uint, ConnectionEndPoint> _connections;

    // qthread
};

#endif // BUYINGSESSIONCONTEXT_H
