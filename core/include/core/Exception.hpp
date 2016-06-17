/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_EXCEPTION_HPP
#define JOYSTREAM_CORE_EXCEPTION_HPP

#include <core/Node.hpp>

#include <exception>
#include <QJsonValue>

namespace joystream {
namespace core {
namespace exception {

    class MissingJSONKey : public std::runtime_error {

    public:

        MissingJSONKey(const std::string & key)
            : key(key) {
            message += "Missing key " + key;
        }

        std::string key;

        virtual const char* what() const {
            return message.c_str();
        }

    private:

        std::string message;
    };

    class InvalidJSONValueType : public std::runtime_error {

    public:

        InvalidJSONValueType(const std::string & key, QJsonValue::Type expected, QJsonValue::Type found)
            : key(key)
            , expected(expected)
            , found(found) {

            message += "Found value type "
                    + toString(found)
                    + ", but expected type "
                    + toString(expected)
                    + " for key"
                    + key;
        }

        virtual const char* what() const {
            return message.c_str();
        }

        std::string key;

        QJsonValue::Type expected;

        QJsonValue::Type found;

    private:

        static std::string toString(QJsonValue::Type type) {

            switch(type) {

                case QJsonValue::Type::Array: return std::string("Array");
                case QJsonValue::Type::Bool: return std::string("Bool");
                case QJsonValue::Type::Double: return std::string("Double");
                case QJsonValue::Type::Null: return std::string("Null");
                case QJsonValue::Type::Object: return std::string("Object");
                case QJsonValue::Type::String: return std::string("String");
                case QJsonValue::Type::Undefined: return std::string("Undefined");

                default:
                    assert(false);
            }
        }

        std::string message;

    };

    class CanOnlyStartStoppedNode : public std::runtime_error {

    public:

        CanOnlyStartStoppedNode(Node::State badState)
            : badState(badState) {
            message += "Node can only be started when stopped.";
        }


        // The state in the node
        Node::State badState;

        virtual const char* what() const {
            return message.c_str();
        }

    private:

        std::string message;


    };
}
}
}

#endif // JOYSTREAM_CORE_EXCEPTION_HPP
