#include "PluginStatusAlert.hpp"

PluginStatusAlert::PluginStatusAlert(double balance)
    : _balance(balance) {
}

PluginStatusAlert::PluginStatusAlert(const PluginStatusAlert & alert)
    : _balance(alert.balance()) {
}

int PluginStatusAlert::type() const {
    return alert_type;
}

char const* PluginStatusAlert::what() const {
    return "PluginStatusAlert";
}

std::string PluginStatusAlert::message() const {
    return std::string("PluginStatusAlert::message: IMPLEMENT LATER");
}

int PluginStatusAlert::category() const {
    return libtorrent::alert::stats_notification;
}

std::auto_ptr<libtorrent::alert> PluginStatusAlert::clone() const {
    return std::auto_ptr<alert>(new PluginStatusAlert(*this));
}

double PluginStatusAlert::balance() const {
    return _balance;
}
