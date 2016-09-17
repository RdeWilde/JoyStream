/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 22 2016
 */

#include <gui/SessionDialog.hpp>
#include <gui/SellingModeSessionWidget.hpp>
#include <gui/BuyingModeSessionWidget.hpp>
#include <gui/Common.hpp>
#include "ui_SessionDialog.h"
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace gui {

SessionDialog::SessionDialog(QWidget * parent,
                             const BitcoinDisplaySettings * settings,
                             const std::shared_ptr<core::Session> & session)
    : QDialog(parent)
    , _settings(settings)
    , _session(session) {
}

SessionDialog * SessionDialog::create(QWidget * parent,
                                      const BitcoinDisplaySettings * settings,
                                      const std::shared_ptr<core::Session> & session) {

    SessionDialog * dialog = new SessionDialog(parent, settings, session);

    dialog->updateSessionMode();

    return dialog;
}

SessionDialog::~SessionDialog()
{
    delete ui;
}

void SessionDialog::updateSessionMode() {

    if(std::shared_ptr<core::Session> session = _session.lock()) {

        if(session->mode() == protocol_session::SessionMode::buying) {

            ui->sessionModeValueLabel->setText(Common::toString(protocol_session::SessionMode::buying));

            _buyingWidget.release();
            _sellingWidget = std::unique_ptr<SellingModeSessionWidget>(SellingModeSessionWidget::create(this, _settings, session));

            // Position widget

            // Resize dialog


        } else if(session->mode() == protocol_session::SessionMode::selling) {

            ui->sessionModeValueLabel->setText(Common::toString(protocol_session::SessionMode::selling));

            _buyingWidget = std::unique_ptr<BuyingModeSessionWidget>(BuyingModeSessionWidget::create(this, _settings, session));
            _sellingWidget.release();

            // Position widget

            // Resize dialog

        } else {

            std::clog << "Session in wrong mode, closing SessionDialog dialog." << std::endl;

            // Show message box?

            // Close dialog
            done(0);
        }

    } else {

        std::clog << "Session has expired, closing SessionDialog dialog." << std::endl;

        // Show message box?

        // Close dialog
        done(0);
    }
}

}
}
}
