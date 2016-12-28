/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 18 2015
 */

#include <common/LibtorrentUtilities.hpp>

#include <libtorrent/socket_io.hpp> // libtorrent::print_endpoint

uint qHash(const libtorrent::tcp::endpoint & endpoint) {

    // Convert to std::string
    std::string endPointStdString = libtorrent::print_endpoint(endpoint);

    // Hash it
    std::hash<std::string> str_hash;
    return str_hash(endPointStdString);
}

/**
uint qHash(const libtorrent::peer_request & request) {

    // Encode as string, since encoding as int requires knowing size of either request dimension,
    // which varies across torrents
    QString stringEncoding = QString::number(request.piece) + ":" + QString::number(request.start);

    // Hash it
    return qHash(stringEncoding);
}
*/

/*
#include "Utilities.hpp"
#include "extension/Message/MessageType.hpp"

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp> // bdecode()

#include <QString>

#include <fstream>
#include <vector>

#include <libtorrent/socket_io.hpp> // libtorrent::print_endpoint
#include <QHash>

bool Utilities::loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry) {

    // Open file at the end, so we can get size
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ifstream::ate);

    // Get size of file
    int sizeOfFile = (int)file.tellg();

    // Rewind position
    file.seekg(0);

    // Allocate space for raw file
    std::vector<char> dataVector(sizeOfFile, 0);

    // Read entire file into vector
    file.read(&dataVector[0], dataVector.size());

    // Close file
    file.close();

    // Check that entire file was read,
    // if not, indicate failure
    if(!file)
        return false;

    // Bendecode entry
    destinationEntry = libtorrent::bdecode(dataVector.begin(), dataVector.end());

    // Indicate success
    return true;
}

bool Utilities::saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry) {

    // Bencode entry
    std::vector<char> bencodedEntry;
    bencode(std::back_inserter(bencodedEntry), sourceEntry);

    // Open file, overwrite if present
    std::ofstream file(fileName, std::ios::out | std::ios::binary | std::ios::trunc);

    // Save to file
    file.write(reinterpret_cast<const char*>(&bencodedEntry[0]), bencodedEntry.size());

    // Check that entire file was written
    // if not, indicate failure
    if(!file)
        return false;

    // Close file
    file.close();

    // Indicate success
    return true;
}

const std::map<MessageType, const char *> Utilities::messageTypeToNameMapping() {

    // Create type to name mapping
    std::map<MessageType, const char *> m;

    // Setup mapping
    m[MessageType::observe] = "observe";
    m[MessageType::buy] = "buy";
    m[MessageType::sell] = "sell";
    m[MessageType::join_contract] = "join_contract";
    m[MessageType::joining_contract] = "joining_contract";
    m[MessageType::sign_refund] = "sign_refund";
    m[MessageType::refund_signed] = "refund_signed";
    m[MessageType::ready] = "ready";
    m[MessageType::request_full_piece] = "request_full_piece";
    m[MessageType::full_piece] = "full_piece";
    m[MessageType::payment] = "payment";


    // Return mapping
    return m;
}

const char * Utilities::messageName(MessageType type) {

    // Get mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Find name of type
    std::map<MessageType, const char *>::const_iterator i = m.find(type);

    if(i == m.end())
        throw std::runtime_error("No name registered for type, messageTypeToNameMapping() does not contain all types!");
    else
        return i->second;
}

MessageType Utilities::messageType(const std::string & messageName) {

    // Invert messageTypeToNameMapping()

    // Get type to name mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Iterate map and find preimage of messageName
    for(std::map<MessageType, const char *>::const_iterator i = m.begin(),
            end(m.end()); i != end;i++) {

        // return type if name matches
        if(messageName.compare(i->second) == 0)
            return i->first;
    }

    throw std::runtime_error("Unknown message name passed.");
}

const std::set<MessageType> Utilities::allMessageTypes() {

    // Create set
    std::set<MessageType> m;

    // Populate set by taking keys from messageTypeToNameMapping()
    std::map<MessageType, const char *> x = messageTypeToNameMapping();

    // Iterate map and find preimage of messageName
    for(std::map<MessageType, const char *>::const_iterator i = x.begin(),
            end(x.end()); i != end;i++)
        m.insert(i->first);

    // Return mappings
    return m;
}

int Utilities::numberOfMessageTypes() {
    return allMessageTypes().size();
}

#include <QJsonObject>
#include <QJsonArray>

bool Utilities::GET_BOOL(const QJsonObject & dictionary, const QString & key) {

    // Get value
    QJsonValue vValue = dictionary[key];

    // Check that it has correct type
    if(vValue.type() != QJsonValue::Bool) {
        QString str = key + " key does not exist or is not of type QJsonValue::Bool.";
        throw new std::runtime_error(str.toLatin1().constData());
    }

    return vValue.toBool();
}

double Utilities::GET_DOUBLE(const QJsonObject & dictionary, const QString & key) {

    // Get value
    QJsonValue vValue = dictionary[key];

    // Check that it has correct type
    if(vValue.type() != QJsonValue::Double) {
        QString str = key + " key does not exist or is not of type QJsonValue::Double.";
        throw new std::runtime_error(str.toLatin1().constData());
    }

    return vValue.toDouble();
}

QString Utilities::GET_STRING(const QJsonObject & dictionary, const QString & key) {

    // Get value
    QJsonValue vValue = dictionary[key];

    // Check that it has correct type
    if(vValue.type() != QJsonValue::String) {
        QString str = key + " key does not exist or is not of type QJsonValue::String.";
        throw new std::runtime_error(str.toLatin1().constData());
    }

    return vValue.toString();
}

QJsonArray Utilities::GET_ARRAY(const QJsonObject & dictionary, const QString & key) {

    // Get value
    QJsonValue vValue = dictionary[key];

    // Check that it has correct type
    if(vValue.type() != QJsonValue::Array) {
        QString str = key + " key does not exist or is not of type QJsonValue::Array.";
        throw new std::runtime_error(str.toLatin1().constData());
    }

    return vValue.toArray();
}

QJsonObject Utilities::GET_OBJECT(const QJsonObject & dictionary, const QString & key) {

    // Get value
    QJsonValue vValue = dictionary[key];

    // Check that it has correct type
    if(vValue.type() != QJsonValue::Object) {
        QString str = key + " key does not exist or is not of type QJsonValue::Object.";
        throw new std::runtime_error(str.toLatin1().constData());
    }

    return vValue.toObject();
}

PluginInstalled Utilities::PluginModeToPluginInstalled(PluginMode mode) {

    switch(mode) {

        case PluginMode::Buyer:
            return PluginInstalled::Buyer;

        case PluginMode::Observer:
            return PluginInstalled::Observer;

        case PluginMode::Seller:
            return PluginInstalled::Seller;
    }

}

#include <QtMath>
#include <QTime>

uint8_t Utilities::hoursInSeconds(uint32_t seconds) {
    return qFloor(seconds / 3600);
}

uint8_t Utilities::minutesInSeconds(uint32_t seconds) {
    return qFloor(((seconds - 3600 * Utilities::hoursInSeconds(seconds)) / 60));
}

uint8_t Utilities::secondsInSeconds(uint32_t seconds) {
    return seconds - Utilities::hoursInSeconds(seconds)*3600 - Utilities::minutesInSeconds(seconds)*60;
}

QTime Utilities::secondsToQTime(uint32_t seconds) {

    uint8_t h = Utilities::hoursInSeconds(seconds);
    uint8_t m = Utilities::minutesInSeconds(seconds);
    uint8_t s = Utilities::secondsInSeconds(seconds);

    return QTime(h,m,s);
}
*/
