#include <app_kit/Settings.hpp>

#include <QJsonObject>

namespace joystream {
namespace appkit {

QJsonValue Settings::toJson() const {
    QJsonObject value;

    value["bitcoinNodeHost"] = QJsonValue(QString::fromStdString(bitcoinNodeHost));
    value["bitcoinNodePort"] = QJsonValue(bitcoinNodePort);

    return value;
}

Settings Settings::fromJson(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting json object value");

    QJsonObject o = value.toObject();
    Settings settings;

    settings.bitcoinNodeHost = o["bitcoinNodeHost"].toString().toStdString();
    settings.bitcoinNodePort = o["bitcoinNodePort"].toInt();

    return settings;

}
}
}
