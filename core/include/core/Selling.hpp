/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 17 2016
 */

#ifndef JOYSTREAM_CORE_SELLING_HPP
#define JOYSTREAM_CORE_SELLING_HPP

#include <protocol_session/protocol_session.hpp>

#include <QObject>

namespace joystream {
namespace core {

/**
 * @brief Handle for selling sub state of session
 * @note Detect expiry by listening to the Session::modeChanged signal, which
 * happens when mode changes away from selling.
 */
class Selling : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Selling(const protocol_session::SellingPolicy & policy,
            const protocol_wire::SellerTerms & terms);

    static Selling * create(const protocol_session::status::Selling & status);

    protocol_session::SellingPolicy policy() const noexcept;

    protocol_wire::SellerTerms terms() const noexcept;

signals:

    void policyChanged(const protocol_session::SellingPolicy &);

    void termsChanged(const protocol_wire::SellerTerms &);

private:

    friend class Session;

    void update(const protocol_session::status::Selling &);

    protocol_session::SellingPolicy _policy;

    protocol_wire::SellerTerms _terms;
};

}
}

#endif // JOYSTREAM_CORE_SELLING_HPP
