#ifndef PLUGIN_STATUS_ALERT_HPP
#define PLUGIN_STATUS_ALERT_HPP

#include "AlertTypes.hpp"

#include <libtorrent/alert.hpp>

class PluginStatusAlert : public libtorrent::alert
{
public:

    // Public member required for alert_cast
    const static int alert_type = PLUGIN_STATUS_ALERT_ID;

    // Constructors
    PluginStatusAlert(double balance);
    PluginStatusAlert(const PluginStatusAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters
    double balance() const;

private:

    double _balance;
};

#endif // PLUGIN_STATUS_ALERT_HPP
