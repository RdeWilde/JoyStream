#include "CannnotOpenLogFile.hpp"

CannnotOpenLogFile::CannnotOpenLogFile(const QFile * file) : file_(file) {
    whatMessage += file_->fileName().toStdString() + " is already registered with category logger.";
}

const char * CannnotOpenLogFile::what() const {
    return whatMessage.c_str();
}
