#ifndef DUPLICATE_CATEGORY_LOG_HPP
#define DUPLICATE_CATEGORY_LOG_HPP

#include <exception>
#include <QString>

class DuplicateCategoryLog : public std::exception
{
private:

    // Message returned in what()
    std::string whatMessage;

public:

    QString name_;

    // Constructor
    DuplicateCategoryLog(QString & name);

    // Message
    virtual const char * what() const throw();
};

#endif // DUPLICATE_CATEGORY_LOG_HPP
