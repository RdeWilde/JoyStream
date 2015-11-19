/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 14 November 2015
 */

#include <Mixpanel/Event.hpp>

#include <QHostAddress>
#include <QJsonObject>
#include <QDateTime>
#include <QJsonDocument>
#include <QNetworkRequest>

namespace Mixpanel {

Event::Properties::Properties(const QString & token)
    : _token(token) {
}

QJsonObject Event::Properties::toJSON() const {

    if(contains("token"))
        throw std::runtime_error("Invalid properties, contains token key.");

    QJsonObject json;

    json["token"] = _token;

    for(QMap<QString, QString>::const_iterator i = constBegin();
        i !=  constEnd();i++)
        json[i.key()] = i.value();

    return json;
}

void Event::Properties::setDistinctId(const QString & distinctId) {
    insert("distinct_id", distinctId);
}

void Event::Properties::setTime(const QDateTime & time) {
    insert("time", QString::number(time.toMSecsSinceEpoch()));
}

void Event::Properties::setIp(const QHostAddress & ip) {
    insert("ip", ip.toString());
}

Event::Event(const QString & event, const Properties & properties)
    : _event(event)
    , _properties(properties) {
}

QNetworkRequest Event::toTrackingRequest() const {
    return QNetworkRequest(QUrl("https://api.mixpanel.com/track/?data=" + toData()));
}

QJsonObject Event::toJSON() const {

    QJsonObject data;

    data["event"] = _event;
    data["properties"] = _properties.toJSON();

    return data;
}

QString Event::toData() const {

    QJsonDocument doc(toJSON());
    QByteArray raw = doc.toJson();

    QByteArray encoded = raw.toBase64();

    return encoded;
}

Event::Properties Event::properties() const {
    return _properties;
}

void Event::setProperties(const Properties & properties) {
    _properties = properties;
}

}
