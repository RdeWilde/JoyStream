#ifndef CATEGORY_LOGGER_MANAGER_HPP
#define CATEGORY_LOGGER_MANAGER_HPP

#include <QString>
#include <QFile>
#include <QLoggingCategory>
#include <QtGlobal>
#include <QMutex>

class CategoryLoggerManager {

private:

    class Category {

        public:

            // File where messages are outputted
            QFile file_;

            // Logger for category
            QLoggingCategory logger_;

            // Whether standard handler should be used
            bool chainStandardHandler_;

            // Whether message should also output message to stdout/stderr
            bool useStandardOutput_;

            // Constructor
            Category(QFile & file, QLoggingCategory & logger, bool chainStandardHandler, bool useStandardOutput);
    };

    // Map name of category to category
    std::map<QString, CategoryLoggerManager::Category> logs;

    // Defualt message handler
    QtMessageHandler defaultHandler;

    // Synchronizes all threads accessing public routines
    QMutex mutex;

public:

    // Constructor
    CategoryLoggerManager();

    // Destructor
    ~CategoryLoggerManager();

    // Creates a new category
    QLoggingCategory createCategoryLogger(const QString & name, bool chainStandardHandler, bool useStandardOutput);

    // QtMessageHandler
    void handler(QtMsgType type, const QMessageLogContext & messageLogContext, const QString & msg);
};

#endif // CATEGORY_LOGGER_MANAGER_HPP
