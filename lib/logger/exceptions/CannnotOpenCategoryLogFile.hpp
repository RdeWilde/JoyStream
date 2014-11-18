#ifndef CANNNOT_OPEN_CATEGORY_LOG_FILE_HPP
#define CANNNOT_OPEN_CATEGORY_LOG_FILE_HPP

#include <exception>
#include <QFile>

class CannnotOpenCategoryLogFile : public std::exception
{
private:

    // Message returned in what()
    std::string whatMessage;

public:

    QFile file_;

    // Constructor
    CannnotOpenCategoryLogFile(QFile & file);

    // Message
    virtual const char * what() const throw();
};

#endif // CANNNOT_OPEN_CATEGORY_LOG_FILE_HPP
