/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 23 2017
 */

 #ifndef JOYSTREAM_NODE_COMMON_HPP
 #define JOYSTREAM_NODE_COMMON_HPP

 #include <nan.h>

 #include <boost/optional.hpp>

 namespace libtorrent {
   class alert;
 namespace node {

   /**
    * @brief Alert converter for this plugin.
    * @param a alert for conversion.
    * @param o object to which alert should be converted.
    * @param success will be written to to indicate whether the alert was recognized, and written to ´o´.
    * @return optional encoded alert
    */
   typedef boost::optional<v8::Local<v8::Object>>(*AlertDecoder)(const libtorrent::alert *);

 }
 }

 #endif // JOYSTREAM_NODE_COMMON_HPP
