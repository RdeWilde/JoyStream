/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "LoggerManager.hpp"
#include "exceptions/DuplicateLog.hpp"
#include "exceptions/CannnotOpenLogFile.hpp"

#include <QTextStream>
#include <QMessageLogContext>
#include <iostream>
#include <QDir>

LoggerManager global_log_manager;

void handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg);

// Manager constructor
LoggerManager::LoggerManager() {

    // Alter message handler, and save default
    defaultHandler = qInstallMessageHandler(handler);
}

// Manager destructor
LoggerManager::~LoggerManager() {

    // Iterate loggers and free resources
    for(std::map<QString, LoggerManager::Category>::iterator i = loggers.begin(),
        end(loggers.end()); i != end; i++) {

        // Get logger
        LoggerManager::Category & category = i->second;

        // Free file object
        delete category.file;

        // Free category object
        delete category.qCategory;
    }

    // Reinstall default handler
    qInstallMessageHandler(defaultHandler);
}

QLoggingCategory * LoggerManager::createLogger(const QString & name, bool useStandardOutput = false, bool chainStandardHandler = false) {

    // Acquire lock
    mutex.lock();

    // If is already registered, then throw exception
    if(loggers.count(name) > 0) {

        mutex.unlock();
        throw DuplicateLog(name);   
    }

    // Create category logger
    LoggerManager::Category category;
    category.name = name;
    category.file = new QFile(QDir::homePath() + QDir::separator() + name + QString(".txt"));

    // Need const char * to name which persist, i.e. on heap, thanks QLoggingCategory :/
    const QByteArray * rawByteArrayOnHeap = new QByteArray(name.toLatin1());

    category.qCategory = new QLoggingCategory(rawByteArrayOnHeap->constData());
    category.chainStandardHandler = chainStandardHandler;
    category.useStandardOutput = useStandardOutput;

    // Open file
    if(!category.file->open(QIODevice::WriteOnly | QIODevice::Text)) {

        mutex.unlock();
        throw CannnotOpenLogFile(category.file);
    }

    // Save in logs map
    loggers[name] = category;

    // Release lock
    mutex.unlock();

    // Return QLoggingCategory pointer
    return category.qCategory;
}

// QtMessageHandler
//- is BOUND TO global log_manager, since
// Change later
void handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg) {

    // Acquire lock
    global_log_manager.mutex.lock();

    //std::cerr << "Acquired lock." << std::endl;

    // If category is not registered, then logging is not coming from one of our category loggers
    if(global_log_manager.loggers.count(messageLogContext.category) == 0) {

        // Call default handler
        global_log_manager.defaultHandler(type, messageLogContext, msg);

        // Release lock
        global_log_manager.mutex.unlock();
        //std::cerr << "Release lock." << std::endl;

        // This is all we do
        return;
    }

    // Get category
    LoggerManager::Category & category = global_log_manager.loggers[messageLogContext.category];

    // Build message
    QString messageType;

    switch(type) {

        case QtDebugMsg:
            messageType = "Debug: ";
            break;
        case QtWarningMsg:
            messageType = "Warning: ";
            break;
        case QtCriticalMsg:
            messageType = "Critical: ";
            break;
        case QtFatalMsg:
            messageType = "Fatal: ";
    }

    /**
    QString messageSource;
    messageSource.append("[")
            .append(messageLogContext.line)
            .append("@")
            .append(messageLogContext.function)
            .append("@")
            .append(messageLogContext.file)
            .append("] ");
    */

    // Write to file
    QTextStream(category.file) << msg << "\n"; // << messageSource << messageType

    // and flush
    category.file->flush();

    // Write to screen
    if(category.useStandardOutput)
        std::cerr << "[" << messageLogContext.category << "]: "
                  //<< messageType.toStdString().c_str()
                  << msg.toStdString().c_str()
                  << std::endl;

    // Pass to default handler
    if(category.chainStandardHandler)
        global_log_manager.defaultHandler(type, messageLogContext, msg);

    // Abort if we received fatal message
    if(type == QtFatalMsg)
        abort();

    //std::cerr << "Release lock." << std::endl;

    // Release lock
    global_log_manager.mutex.unlock();
}
