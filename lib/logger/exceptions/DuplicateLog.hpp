#ifndef DUPLICATE_LOG_HPP
#define DUPLICATE_LOG_HPP

#include <exception>
#include <QString>

class DuplicateLog : public std::exception
{
private:

    // Message returned in what()
    std::string whatMessage;

public:

    QString name_;

    // Constructor
    DuplicateLog(const QString & name);

    // Message
    virtual const char * what() const throw();
};

#endif // DUPLICATE_LOG_HPP
