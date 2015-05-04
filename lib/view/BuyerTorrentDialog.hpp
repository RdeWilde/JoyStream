#ifndef BUYER_TORRENT_DIALOG_HPP
#define BUYER_TORRENT_DIALOG_HPP

namespace Ui {
class BuyerTorrentDialog;
}

#include <libtorrent/socket.hpp> // tcp::endpoint

#include <QDialog>
#include <QStandardItemModel>
#include <QMenu>
#include <QModelIndex>
#include <QMap>

class QStandardItemModel;
class QMenu;

/**
 * @brief The BuyerTorrentDialog class
 */
class BuyerTorrentDialog : public QDialog
{
    Q_OBJECT

public:

    /**
     * View-model of a peer seller
     */
    class PeerViewModel {

        static const char * columnTitles[];
        static const int numberOfColumns;

    public:

        // Default constructor
        PeerViewModel();

        // Constructor from members
        PeerViewModel(const libtorrent::tcp::endpoint & endPoint,
                      quint32 slotIndex,
                      quint64 funds,
                      quint64 price,
                      quint64 balance,
                      quint32 bytesPrSecond);

        // Update
        void updateBalance(quint64 balance);
        void updateSpeed(quint32 bytesPrSecond);


    private:

        // Endpoint of peer
        libtorrent::tcp::endpoint _endPoint;

        // Model items, have to be pointers since QStandardItemModel takes ownership of
        // objects and deletes them.
        QStandardItem * _endPointItem,
                      * _speedItem,
                      * _channelOutputIndexItem,
                      * _fundsItem,
                      * _expirationTimeItem,
                      * _priceItem,
                      * _balanceItem;
    };

    //explicit BuyerTorrentDialog(QWidget *parent = 0);

    //
    BuyerTorrentDialog();

    // Destructor
    ~BuyerTorrentDialog();

private:
    Ui::BuyerTorrentDialog *ui;

    /*
     * View-models
     */

    // Peers table
    QStandardItemModel * _peersTableViewModel; // View model
    QMenu * _peersTableContextMenu; // Context menu
    QModelIndex _peersTableLastIndexClicked; // Last model index for mouse click

    // Maps end point to cooresponding peer view model
    QMap<libtorrent::tcp::endpoint, PeerViewModel> _peerViewModels;

};

#endif // BUYER_TORRENT_DIALOG_HPP
