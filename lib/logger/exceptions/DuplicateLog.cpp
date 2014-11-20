#include "DuplicateLog.hpp"

DuplicateLog::DuplicateLog(const QString & name)
    : name_(name)
{
    whatMessage += name_.toStdString() + " is already registered with category logger.";
}

const char * DuplicateLog::what() const {
    return whatMessage.c_str();
}
