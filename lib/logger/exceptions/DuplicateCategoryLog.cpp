#include "DuplicateCategoryLog.hpp"

DuplicateCategoryLog::DuplicateCategoryLog(QString & name) //, std::map<QString, CategoryLoggerManager::Category> & logs)
    : name_(name)
    //, logs_(logs)
{
    whatMessage += name_.toStdString() + " is already registered with category logger.";
}

const char * DuplicateCategoryLog::what() const {
    return whatMessage.c_str();
}
