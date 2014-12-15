#ifndef BEP_SUPPORT_STATUS_HPP
#define BEP_SUPPORT_STATUS_HPP

// BEP support state indicator, is used for both regular
// 1) BitTorrent handshake
// 2) BEP10 handshake
enum class BEPSupportStatus {

    // Before handshake
    unknown,

    // After handshake
    supported,
    not_supported
};

#endif // BEP_SUPPORT_STATUS_HPP
