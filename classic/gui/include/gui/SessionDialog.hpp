/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 22 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP

#include <QDialog>

#include <memory>

class BitcoinDisplaySettings;

namespace Ui {
    class SessionDialog;
}

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
}
namespace core {
    class Session;
}
namespace classic {
namespace gui {

class SellingModeSessionWidget;
class BuyingModeSessionWidget;

class SessionDialog : public QDialog
{
    Q_OBJECT


private:

    explicit SessionDialog(QWidget * parent,
                           const BitcoinDisplaySettings * settings,
                           const std::shared_ptr<core::Session> & session);

public:

    static SessionDialog * create(QWidget * parent,
                                  const BitcoinDisplaySettings * settings,
                                  const std::shared_ptr<core::Session> & session);

    ~SessionDialog();

public slots:

    void updateSessionMode();

private:

    Ui::SessionDialog * ui;

    const BitcoinDisplaySettings * _settings;

    // Use std::optional when its available
    std::unique_ptr<SellingModeSessionWidget> _sellingWidget;
    std::unique_ptr<BuyingModeSessionWidget> _buyingWidget;

    std::weak_ptr<core::Session> _session;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SESSIONDIALOG_HPP
