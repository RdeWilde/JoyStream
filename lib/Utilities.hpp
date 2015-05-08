#ifndef UTILITIES_HPP
#define UTILITIES_HPP

namespace libtorrent {
    class entry;
}

enum class MessageType;

#include <set>
#include <map>

// Required for QSet<libtorrent::tcp::endpoint>
#include <QtGlobal> // uint

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint
uint qHash(const libtorrent::tcp::endpoint & endpoint);

#include <libtorrent/peer_connection.hpp> // libtorrent::peer_request
//#include <libtorrent/peer_request.hpp> // cannot be included directly, since TORRENT_EXPORT was not defined
uint qHash(const libtorrent::peer_request & request);

#include <QJsonValue>

//class QJsonObject;

#include "extension/PluginMode.hpp"
#include "controller/PluginInstalled.hpp"

class QTime;

/**
 * Static utility functions
 */
class Utilities
{
public:

    // Opens file, reads contents, debencodes into destinationEntry
    static bool loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry);

    // Bencodes dictionary, opens file, writes to file
    static bool saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry);

    // Mapping from type to name
    static const std::map<MessageType, const char *> messageTypeToNameMapping();

    // Name from type
    static const char * messageName(MessageType type);

    // Type from name
    static MessageType messageType(const std::string & messageName);

    // All messages
    static const std::set<MessageType> allMessageTypes();

    // Count the number of message types
    static int numberOfMessageTypes();

    // Get value from dictionary of given type
    static bool GET_BOOL(const QJsonObject & dictionary, const QString & key);
    static double GET_DOUBLE(const QJsonObject & dictionary, const QString & key);
    static QString GET_STRING(const QJsonObject & dictionary, const QString & key);
    static QJsonArray GET_ARRAY(const QJsonObject & dictionary, const QString & key);
    static QJsonObject GET_OBJECT(const QJsonObject & dictionary, const QString & key);

    // Conversion routine
    static PluginInstalled PluginModeToPluginInstalled(PluginMode mode);

    // Converts x-> #units in x
    static quint8 hoursInSeconds(quint32 seconds);
    static quint8 minutesInSeconds(quint32 seconds);
    static quint8 secondsInSeconds(quint32 seconds);

};

#endif // UTILITIES_HPP
