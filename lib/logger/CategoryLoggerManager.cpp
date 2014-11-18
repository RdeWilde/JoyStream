#include "CategoryLoggerManager.hpp"
#include "exceptions/DuplicateCategoryLog.hpp"
#include "exceptions/CannnotOpenCategoryLogFile.hpp"

// Inner class constructor
CategoryLoggerManager::Category::Category(QFile & file, QLoggingCategory & logger, bool chainStandardHandler, bool useStandardOutput)
{

    /*
     *     : CategoryLoggerManager::Category::file_(file)
    , CategoryLoggerManager::Category::logger_(logger)
    , CategoryLoggerManager::Category::chainStandardHandler_(chainStandardHandler)
    , CategoryLoggerManager::Category::useStandardOutput_(useStandardOutput)
    */

    file_ = file;
    logger_ = logger;
    chainStandardHandler_ = chainStandardHandler;
    useStandardOutput_ = useStandardOutput;
}

// Manager constructor
CategoryLoggerManager::CategoryLoggerManager() {

    // Alter message handler, and save default
    defaultHandler = qInstallMessageHandler(this->handler);
}

// Manager destructor
CategoryLoggerManager::~CategoryLoggerManager()
{ }

QLoggingCategory CategoryLoggerManager::createCategoryLogger(const QString & name, bool chainStandardHandler = false, bool useStandardOutput = false) {

    // Acquire lock
    mutex.lock();

    // If is already registered, then throw exception
    if(logs.count(name) > 0)
        throw DuplicateCategoryLog(name);

    // Open file
    QFile file(name);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CannnotOpenCategoryLogFile(file);

    // Create category
    Category category(file, QLoggingCategory(name), chainStandardHandler, useStandardOutput);

    // Save in logs map
    logs[name] = category;

    // Release lock
    mutex.unlock();
}

void CategoryLoggerManager::handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg) {

    // Acquire lock
    mutex.lock();

    // Get category
    Category category;

    // If category is not registered, then logging is not coming from one of our category loggers
    if(logs.count(messageLogContex.category) > 0)
        category = logs[messageLogContex.category];
    else {

        // Call default handler
        defaultHandler(type, messageLogContext, msg);

        // This is all we do
        return;
    }

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

    message += "["
            + messageLogContext.line
            + "@"
            + messageLogContext.function
            + "@"
            + messageLogContext.file
            + "] "
            + msg;

    // Write to file
    QTextStream(&category.file_) << message << std::endl;
    category.file_.flush();

    // Write to screen
    if(category.useStandardOutput_)
        std::cerr << message.toStdString().c_str() << std::endl;

    // Pass to default handler
    if(category.chainStandardHandler_)
        defaultHandler(type, messageLogContext, msg);

    // Abort if we received fatal message
    if(type == QtFatalMsg)
        abort();

    // Realse lock
    mutex.unlock();
}
