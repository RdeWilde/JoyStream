#include "BuyerTorrentDialog.hpp"
#include "ui_BuyerTorrentDialog.h"

#include <libtorrent/socket_io.hpp> // print_endpoint

/**
 * BuyerTorrentDialog::PeerViewModel
 */

const char * BuyerTorrentDialog::PeerViewModel::columnTitles[] = {"Peer", "Status", "Speed", "Contract Slot", "Funds", "Expires" , "Price", "Balance"};
const int BuyerTorrentDialog::PeerViewModel::numberOfColumns = sizeof(BuyerTorrentDialog::PeerViewModel::columnTitles)/sizeof(char *);


BuyerTorrentDialog::PeerViewModel::PeerViewModel()
    : _endPointItem(0)
    //, _slotIndexItem(0)
    , _fundsItem(0)
    , _priceItem(0)
    , _balanceItem(0)
    , _speedItem(0) {
}

// Constructor from members
BuyerTorrentDialog::PeerViewModel::PeerViewModel(const libtorrent::tcp::endpoint & endPoint,
                                                  quint32 slotIndex,
                                                  quint64 funds,
                                                  quint64 price,
                                                  quint64 balance,
                                                  quint32 bytesPrSecond)
                                                  //QStandardItemModel * peersTableViewModel)
    : _endPoint(endPoint)
    , _endPointItem(new QStandardItem(QString::fromStdString(libtorrent::print_endpoint(endPoint))))
    //, _slotIndexItem(new QStandardItem(QString::number(slotIndex)))
    , _fundsItem(new QStandardItem(QString::number(funds)))
    , _priceItem(new QStandardItem(QString::number(price)))
    , _balanceItem(new QStandardItem())
    , _speedItem(new QStandardItem()) {
    //, _peersTableViewModel(peersTableViewModel)
    //, _peersTableContextMenu(0)

    // Set item values
    updateBalance(balance);
    updateSpeed(bytesPrSecond);

    /**
    // Set item data, so this view model is recoverable
    _endPointItem->setData(QVariant::fromValue(this));
    //_slotIndexItem->setData(QVariant::fromValue(this));
    _fundsItem->setData(QVariant::fromValue(this));
    _priceItem->setData(QVariant::fromValue(this));
    _balanceItem->setData(QVariant::fromValue(this));
    _speedItem->setData(QVariant::fromValue(this));

    #include <QMetaType>
    Q_DECLARE_METATYPE(TorrentViewModel*) // QStandardItem::setData(QVariant::fromValue(this))

    */

    // Add as row to _peersTableViewModel
    QList<QStandardItem *> row;

    row.append(_endPointItem);
    //row.append(_slotIndexItem);
    row.append(_fundsItem);
    row.append(_priceItem);
    row.append(_balanceItem);
    row.append(_speedItem);

    //_peersTableViewModel->appendRow(row);
}

void BuyerTorrentDialog::PeerViewModel::updateBalance(quint64 balance) {
    _balanceItem->setText(QString::number(balance));
}

void BuyerTorrentDialog::PeerViewModel::updateSpeed(quint32 bytesPrSecond) {
    _balanceItem->setText(QString::number(bytesPrSecond) + "/s");
}

/**
 * BuyerTorrentDialog
 */


BuyerTorrentDialog::BuyerTorrentDialog() // QWidget *parent
    : //QDialog(parent)
    ui(new Ui::BuyerTorrentDialog) {

    ui->setupUi(this);

    /**

    // Add columns to model view model
    for(int i = 0;i < TorrentViewModel::numberOfColumns;i++)
        _torrentTableViewModel.setHorizontalHeaderItem(i, new QStandardItem(TorrentViewModel::columnTitles[i]));

    // Set table model to view model
    ui->torrentsTable->setModel(&_torrentTableViewModel);

    // Setup context menu capacity on table view
    ui->torrentsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->torrentsTable,
            SIGNAL(customContextMenuRequested(QPoint)),
            this,
            SLOT(showContextMenu(QPoint)));

    // Setup mouse click on torrent table view
    connect(ui->torrentsTable,
            SIGNAL(clicked(const QModelIndex &)),
            this,
            SLOT(torrentTableClicked(const QModelIndex &)));

    // Add menu buttons
    QAction * pauseAction = new QAction("Pause", this);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pauseMenuAction()));
    _torrentTableContextMenu->addAction(pauseAction);

    QAction * startAction = new QAction("Start", this);
    connect(startAction, SIGNAL(triggered()), this, SLOT(startMenuAction()));
    _torrentTableContextMenu->addAction(startAction);

    QAction * removeAction = new QAction("Remove", this);
    connect(removeAction, SIGNAL(triggered()), this, SLOT(removeMenuAction()));
    _torrentTableContextMenu->addAction(removeAction);
    */

}

BuyerTorrentDialog::~BuyerTorrentDialog()
{
    delete ui;
}
