/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 22 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP

#include <QDialog>

class QStandardItemModel;

namespace Ui {
    class SessionDialog;
}

namespace joystream {
namespace classic {
namespace gui {

class SellingModeSessionWidget;
class BuyingModeSessionWidget;

class SessionDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SessionDialog(QWidget * parent);

    ~SessionDialog();

    /// This interface may change later when we start using layouts,
    /// we shouldnt have to store the widget pointers as is now

    /**
     * @brief showSellModeWidget
     * @param widget
     * @note Takes ownership of widget
     */
    void showSellModeWidget(SellingModeSessionWidget * widget);

    /**
     * @brief showBuyModeWidget
     * @param widget
     * @note Takes ownership of widget
     */
    void showBuyModeWidget(BuyingModeSessionWidget * widget);

private:

    Ui::SessionDialog * ui;

    SellingModeSessionWidget * _sellingWidget;
    BuyingModeSessionWidget * _buyingWidget;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP
