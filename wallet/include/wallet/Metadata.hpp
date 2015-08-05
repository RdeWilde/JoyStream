#ifndef METADATA_HPP
#define METADATA_HPP

#include <wallet/Network.hpp>

class QSqlQuery;
class QSqlDatabase;
class QByteArray;
class QDateTime;

// Key value store for wallet metadata
class Metadata {

public:

    static void createTable(QSqlDatabase db);

    static void populateTable(QSqlDatabase db, const QByteArray & seed, Network network, const QDateTime & created);

    static Network getNetwork(QSqlDatabase db);
    static void setNetwork(QSqlDatabase db, Network network);

    static QByteArray getSeed(QSqlDatabase db);
    static void setSeed(QSqlDatabase db, const QByteArray & seed);

    static QDateTime getCreated(QSqlDatabase db);
    static void setCreated(QSqlDatabase db, const QDateTime & created);

private:

    // Key used to store corresponding
    static const QByteArray _networkKey;
    static const QByteArray _seedKey;
    static const QByteArray _createdKey;

    // Encode network as blob
    static QByteArray encodeNetwork(Network network);
    static Network decodeNetwork(const QByteArray & blob);
};

#endif // METADATA_HPP

