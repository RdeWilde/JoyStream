/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 21 2017
 */

#include <nan.h>

#include <boost/shared_ptr.hpp>

namespace libtorrent {
  class alert;
  struct plugin;
namespace node {

/**
 * @brief Wraps libtorrent::plugin, intended to be
 * base class for bindings for libtorrent bindings.
 */
class plugin : public Nan::ObjectWrap {

public:

  /**
   * @brief Alert cnverter for this plugin.
   * @param a alert for conversion.
   * @param o object to which alert should be converted.
   * @param success will be written to to indicate whether the alert was recognized, and written to ´o´.
   */
  typedef void(*converter)(const libtorrent::alert &, v8::Local<v8::Object> & o, bool success);

  /**
   * @brief Get alert converter for this plugin.
   */
  virtual converter getConverter() const noexcept = 0;

  /**
   * @brief Returns reference to this plugin.
   */
  virtual boost::shared_ptr<libtorrent::plugin> getPlugin() const noexcept = 0;

};

}
}
