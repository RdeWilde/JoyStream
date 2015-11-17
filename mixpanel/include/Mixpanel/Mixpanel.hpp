/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 14 November 2015
 */

#ifndef MIXPANEL_CLIENT_HPP
#define MIXPANEL_CLIENT_HPP

namespace Mixpanel {

    class Event;

    // Synchronously send event
    bool sendEvent(const Event & event);

}

#endif // MIXPANEL_CLIENT_HPP
