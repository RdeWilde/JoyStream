#ifndef CATEGORY_LOGGER_MANAGER_HPP
#define CATEGORY_LOGGER_MANAGER_HPP

#include <QString>
#include <QFile>
#include <QLoggingCategory>
#include <QtGlobal> // QtMessageHandler
#include <QMutex>

class LoggerManager {

public:

    class Logger {

        public:

            // Name
            QString name_;

            // File where messages are outputted
            QFile * file_;

            // Logger for category
            QLoggingCategory * logger_;

            // Whether standard handler should be used
            bool chainStandardHandler_;

            // Whether message should also output message to stdout/stderr
            bool useStandardOutput_;

            // Constructor
            Logger(const QString & name, bool chainStandardHandler, bool useStandardOutput);

            // Default Constructor, needed to put objects in std::map
            Logger();

            // Destructor
            ~Logger();
    };

    // Map name of category to category
    // (we use pointer since QLoggingCategory member of Logger cannot be copied around)
    std::map<QString, LoggerManager::Logger> loggers;

    // Defualt message handler
    QtMessageHandler defaultHandler;

    // Synchronizes all threads accessing public routines
    QMutex mutex;

    // Constructor
    LoggerManager();

    // Destructor
    ~LoggerManager();

    // Creates a new category
    QLoggingCategory * createLogger(const QString & name, bool chainStandardHandler, bool useStandardOutput);

};

// Global logger manager used by all
extern LoggerManager global_log_manager;

#endif // CATEGORY_LOGGER_MANAGER_HPP
