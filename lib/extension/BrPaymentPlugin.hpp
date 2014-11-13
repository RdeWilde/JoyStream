
#ifndef BR_PAYMENT_PLUGIN_HPP
#define BR_PAYMENT_PLUGIN_HPP


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

class BrPaymentPlugin : public libtorrent::plugin {

private:

public:

    virtual boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent * newTorrent, void * userData);
    virtual void added(libtorrent::aux::session_impl * session);
    virtual void on_alert(libtorrent::alert const * a);
    virtual void on_tick ();
    virtual bool on_optimistic_unchoke(std::vector<libtorrent::policy::peer*> & peers);
    virtual void save_state(libtorrent::entry & stateEntry) const;
    virtual void load_state(libtorrent::lazy_entry const & stateEntry);
};

#endif
