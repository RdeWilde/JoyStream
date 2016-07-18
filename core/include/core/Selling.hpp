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

class Selling : public QObject {

    Q_OBJECT

private:

    Selling(const protocol_session::status::Selling &);

public:

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
