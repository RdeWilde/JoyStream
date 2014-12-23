#ifndef PLUGIN_REQUEST_HPP
#define PLUGIN_REQUEST_HPP

#include "PluginRequestType.hpp"

class PluginRequest
{
public:

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const = 0;

};

#endif // PLUGIN_REQUEST_HPP
