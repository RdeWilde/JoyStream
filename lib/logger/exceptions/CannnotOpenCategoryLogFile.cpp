#include "CannnotOpenCategoryLogFile.hpp"

CannnotOpenCategoryLogFile::CannnotOpenCategoryLogFile() {
    whatMessage += file_.fileName().toStdString() + " is already registered with category logger.";
}

const char * CannnotOpenCategoryLogFile::what() const {
    return whatMessage.c_str();
}
