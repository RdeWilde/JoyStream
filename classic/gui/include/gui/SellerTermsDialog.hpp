/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SELLERTERMSDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_SELLERTERMSDIALOG_HPP

#include <common/BitcoinRepresentation.hpp>
#include <gui/CurrencyFieldPrefixSettings.hpp>

#include <QDialog>

#include <boost/optional.hpp>

class BitcoinDisplaySettings;

namespace Ui {
    class SellerTermsDialog;
}

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
}
namespace classic {
namespace gui {

class SellerTermsDialog : public QDialog
{
    Q_OBJECT

public:

    struct TorrentInfo {
        unsigned int _numberOfPieces;
        unsigned int _bytesPerPiece;
    };

    enum class DisplayMode {
        simple,
        advanced
    };

    static const int REJECTED_EXIT_CODE;
    static const int SUCCESS_EXIT_CODE;

    SellerTermsDialog(const BitcoinDisplaySettings * settings);

    ~SellerTermsDialog();

    /// Returns correspoding value, throws exception if ui field is not properly set

    // Price is in satoshies (even if settings are in Bitcoin currency)
    quint64 minPrice() const;
    void setMinPrice(quint64 minPrice) const;

    // Time is in seconds
    quint32 minLock() const;
    void setMinLock(quint32 minLock) const;

    quint32 maxNumberOfSellers() const;
    void setMaxNumberOfSellers(quint32 maxNumberOfSellers) const;

    // Price is in satoshies (even if settings are not in Bitcoin currency)
    quint64 minContractFeePerKb() const;
    void setMinContractFeePerKb(quint64 minContractFeePerKb) const;

    // Price is in satoshies
    quint64 settlementFee() const;
    void setSettlementFee(quint64 settlementFee) const;

    protocol_wire::SellerTerms terms() const;

    /// Ui related modifiers

    void setTorrentInfo(const TorrentInfo & torrentInfo);

    DisplayMode displayMode() const noexcept;
    void setDisplayMode(const DisplayMode mode);

private slots:

    void on_displayModeButton_clicked();

    void on_cancelPushButton_clicked();

    void on_okPushButton_clicked();

    void updateValueLabels();

    void updateTitleLabels();

private:

    Ui::SellerTermsDialog *ui;

    DisplayMode _displayMode;

    boost::optional<TorrentInfo> _torrentInfo;

    CurrencyFieldPrefixSettings _minContractFeePerKbPrefixSettings,
                                _minPiecePricePrefixSettings,
                                _pricePerGBPrefixSettings,
                                _settlementFeePrefixSettings,
                                _maxRevenuePrefixSettings;

    /**
     * @brief Compute maximum revenue
     * @param price Satoshi price per peice
     * @param numberOfPieces Number of pieces in torrent
     * @return Maximum revenue (in satoshies)
     */
    static quint64 maxRevenuePrUpload(quint64 price, unsigned int numberOfPieces);

    /**
     * @brief Compute price/GB (in stoshies)
     * @param price Price per piece (in satoshies)
     * @param bytesPerPiece Byter per piece
     * @return Price per GB (in statoshies)
     */
    static quint64 pricePrGB(quint64 price, unsigned int bytesPerPiece);

};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SELLERTERMSDIALOG_HPP
