
#ifndef BITSWAPR_PLUGIN_HPP
#define BITSWAPR_PLUGIN_HPP

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

#include <libtorrent/aux_/session_impl.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/policy.hpp>
#include <libtorrent/peer.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/lazy_entry.hpp>

#include <boost/shared_ptr.hpp>

#include <QThread>
#include <QLoggingCategory>

// Used directing logging to category object.
#define CATEGORY (*category_)

// Forward declaration
class BitSwaprTorrentPlugin;

class BitSwaprPlugin : public libtorrent::plugin {

private:

    // Libtorrent session. Is set by added() call, not constructor
    libtorrent::aux::session_impl * session_;

    // Collection of plugin objects for each torrent added through new_connection()
    std::vector<BitSwaprTorrentPlugin *> torrentPlugins;

    // Logging category
    QLoggingCategory * category_;
public:

    // Constructor
    BitSwaprPlugin(QLoggingCategory * category);

    // Destructor
    ~BitSwaprPlugin();

    virtual boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent * newTorrent, void * userData);
    virtual void added(libtorrent::aux::session_impl * session);
    virtual void on_alert(libtorrent::alert const * a);
    virtual void on_tick();
    virtual bool on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers);
    virtual void save_state(libtorrent::entry & stateEntry) const;
    virtual void load_state(libtorrent::lazy_entry const & stateEntry);
};

#endif
