#ifndef WALLET_NULL_ABLE_HPP
#define WALLET_NULL_ABLE_HPP

#include <QVariant>

namespace Wallet {

    template<class T>
    class NullAble {

    public:

        NullAble();

        NullAble(const T & value);

        //operator==(const NullAble & rhs);

        bool isNull() const;
        QVariant value() const;

    private:

        // I
        bool _isNull;

        // Underlying value
        T _value;

    };

    template<class T>
    NullAble::NullAble()
        : _isNull(true) {
    }

    template<class T>
    NullAble::NullAble(const T & value)
        : _value(value) {
    }

    /**
    template<class T>
    NullAble::operator==(const NullAble & rhs) {
        return
    }
    */

    template<class T>
    bool NullAble::isNull() const {
        return _isNull;
    }

    template<class T>
    QVariant NullAble::value() const {

        if(_isNull)
            return QVariant();
        else
            return QVariant(T);
    }

}

#endif // WALLET_NULL_ABLE_HPP

