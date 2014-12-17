#ifndef PLUGINMODE_HPP
#define PLUGINMODE_HPP

enum class PluginMode {

    // Before torrent has been checked after loading, or before user decides when adding
    // Is only temporary mode
    Undetermined,

    // Neither seller nor buyer, but user has made a decision
    Passive,

    Seller,
    Buyer
};


#endif // PLUGINMODE_HPP
