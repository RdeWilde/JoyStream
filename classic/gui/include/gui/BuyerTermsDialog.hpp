/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYERTERMSDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYERTERMSDIALOG_HPP

#include <common/BitcoinRepresentation.hpp>
#include <gui/CurrencyFieldPrefixSettings.hpp>

#include <QDialog>

#include <boost/optional.hpp>

namespace Ui {
class BuyerTermsDialog;
}

class BitcoinDisplaySettings;

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace classic {
namespace gui {

class BuyerTermsDialog : public QDialog
{
    Q_OBJECT

public:

    enum class DisplayMode {
        simple,
        advanced
    };

    static const int REJECTED_EXIT_CODE;
    static const int SUCCESS_EXIT_CODE;

    BuyerTermsDialog(const BitcoinDisplaySettings * settings);

    ~BuyerTermsDialog();

    /// Returns correspoding value, throws exception if ui field is not properly set

    // Price is in satoshies (even if settings are in Bitcoin currency)
    quint64 maxPrice() const;
    void setMaxPrice(quint64 maxPrice) const;

    // Time is in seconds
    quint32 maxLock() const;
    void setMaxLock(quint32 maxLock) const;

    quint32 minNumberOfSellers() const;
    void setMinNumberOfSellers(quint32 minNumberOfSellers) const;

    // Price is in satoshies (even if settings are not in Bitcoin currency)
    quint64 maxContractFeePerKb() const;
    void setMaxContractFeePerKb(quint64 maxContractFeePerKb) const;

    protocol_wire::BuyerTerms terms() const;

    /// Ui related modifiers

    boost::optional<unsigned int> numberOfPieces() const noexcept;
    void setNumberOfPieces(const boost::optional<unsigned int> & numberOfPieces);

    DisplayMode displayMode() const noexcept;
    void setDisplayMode(const DisplayMode mode);

    // Updates the total expenditure field based on the current configuration values
    void updateTotal();

private slots:

    void on_displayModeButton_clicked();

    void on_cancelPushButton_clicked();

    void on_okPushButton_clicked();

    void updateCurrencyFields();

private:
    Ui::BuyerTermsDialog *ui;

    DisplayMode _displayMode;

    boost::optional<unsigned int> _numberOfPieces;

    CurrencyFieldPrefixSettings _maxContractFeePerKbPrefixSettings,
                                _maxPiecePricePrefixSettings,
                                _totalFundsPrefixSettings;

};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_BUYERTERMSDIALOG_HPP
