/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 20 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP

#include <libtorrent/socket.hpp>
#include <typeindex>
#include <QWidget>

class QStandardItemModel;
class BitcoinDisplaySettings;

namespace Ui {
    class Form;
}

namespace Coin {
    class Transaction;
}

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace protocol_session {
    enum class BuyingState;
    class BuyingPolicy;
}
namespace classic {
namespace gui {

class BuyingModeSessionWidget : public QWidget
{
    Q_OBJECT

public:

    explicit BuyingModeSessionWidget(QWidget * parent,
                                     const BitcoinDisplaySettings * settings);
    ~BuyingModeSessionWidget();

    void setSellerTreeViewModel(QStandardItemModel * model);

public slots:

    void setTerms(const protocol_wire::BuyerTerms & terms);
    void setPolicy(const protocol_session::BuyingPolicy & policy);
    void setState(const protocol_session::BuyingState & state);
    void setContractTx(const Coin::Transaction & tx);

signals:

    void updateTermsClicked();

private:

    Ui::Form *ui;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_BUYINGMODESESSIONWIDGET_HPP
