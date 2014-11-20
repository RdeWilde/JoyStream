#include "LoggerManager.hpp"
#include "exceptions/DuplicateLog.hpp"
#include "exceptions/CannnotOpenLogFile.hpp"

#include <QTextStream>
#include <QMessageLogContext>
#include <iostream>

LoggerManager global_log_manager;

void handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg);

// Inner class constructor
LoggerManager::Logger::Logger(const QString & name, bool chainStandardHandler, bool useStandardOutput)
    : name_(name)
    , file_(new QFile(name))
    , logger_(new QLoggingCategory(name.toStdString().c_str()))
    , chainStandardHandler_(chainStandardHandler)
    , useStandardOutput_(useStandardOutput) {

    // Open file
    if(!file_->open(QIODevice::WriteOnly | QIODevice::Text))
        throw CannnotOpenLogFile(file_);
}

LoggerManager::Logger::Logger()
    : file_(0)
    , logger_(0) {
}

// Inner class destructor
LoggerManager::Logger::~Logger() {

    // Free file object, if non-default constructor used
    if(file_ != 0)
        delete file_;

    // Free logger object, if non-default constructor used
    if(logger_ != 0)
        delete logger_;
}


// Manager constructor
LoggerManager::LoggerManager() {

    // Alter message handler, and save default
    defaultHandler = qInstallMessageHandler(handler);
}

// Manager destructor
LoggerManager::~LoggerManager() {

    // Iterate and free
    for(std::map<QString, LoggerManager::Logger>::iterator i = loggers.begin(),
        end(loggers.end()); i != end;i++)
        delete (i->second).logger_;

    // Reinstall default handler
    qInstallMessageHandler(defaultHandler);
}

QLoggingCategory * LoggerManager::createLogger(const QString & name, bool chainStandardHandler = false, bool useStandardOutput = false) {

    // Acquire lock
    mutex.lock();

    // If is already registered, then throw exception
    if(loggers.count(name) > 0)
        throw DuplicateLog(name);

    // Create category logger
    Logger logger(name, chainStandardHandler, useStandardOutput);

    // Save in logs map
    loggers[name] = logger;

    // Release lock
    mutex.unlock();

    // Return QLoggingCategory pointer
    return logger.logger_;
}

// QtMessageHandler
//- is BOUND TO global log_manager, since
// Change later
void handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg) {

    // Acquire lock
    global_log_manager.mutex.lock();

    // If category is not registered, then logging is not coming from one of our category loggers
    if(global_log_manager.loggers.count(messageLogContext.category) == 0) {

        // Call default handler
        global_log_manager.defaultHandler(type, messageLogContext, msg);

        // This is all we do
        return;
    }

    // Get category
    LoggerManager::Logger & logger = global_log_manager.loggers[messageLogContext.category];

    // Build message
    QString message;

    switch(type) {

        case QtDebugMsg:
            message = "Debug: ";
            break;
        case QtWarningMsg:
            message = "Warning: ";
            break;
        case QtCriticalMsg:
            message = "Critical: ";
            break;
        case QtFatalMsg:
            message = "Fatal: ";
    }

    message.append("[")
            .append(messageLogContext.line)
            .append("@")
            .append(messageLogContext.function)
            .append("@")
            .append(messageLogContext.file)
            .append("] ")
            .append(msg);

    // Write to file
    QTextStream(logger.file_) << message << "\n";

    // and flush
    logger.file_->flush();

    // Write to screen
    if(logger.useStandardOutput_)
        std::cerr << message.toStdString().c_str() << std::endl;

    // Pass to default handler
    if(logger.chainStandardHandler_)
        global_log_manager.defaultHandler(type, messageLogContext, msg);

    // Abort if we received fatal message
    if(type == QtFatalMsg)
        abort();

    // Realse lock
    global_log_manager.mutex.unlock();
}
