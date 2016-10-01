/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP
#define JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP

#include <libtorrent/socket.hpp>
#include <QWidget>
#include <typeindex>

class QAbstractItemModel;
class BitcoinDisplaySettings;

namespace Ui {
    class SellingModeSessionWidget;
}

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
}
namespace protocol_session {
    class SellingPolicy;
}
namespace classic {
namespace gui {

class SellingModeSessionWidget : public QWidget
{
    Q_OBJECT

public:

    explicit SellingModeSessionWidget(QWidget * parent,
                                      const BitcoinDisplaySettings * settings);

    ~SellingModeSessionWidget();

    void setBuyerTreeViewModel(QAbstractItemModel * model);

public slots:

    void setTerms(const protocol_wire::SellerTerms & terms);
    void setPolicy(const protocol_session::SellingPolicy & policy);

private:

    Ui::SellingModeSessionWidget *ui;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SELLINGMODESESSIONWIDGET_HPP
