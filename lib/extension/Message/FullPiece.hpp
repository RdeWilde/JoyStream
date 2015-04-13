#ifndef FULLPIECE_HPP
#define FULLPIECE_HPP

#include "ExtendedMessagePayload.hpp"

#include <QVector>

class FullPiece : public ExtendedMessagePayload
{
public:

    // Default constructor
    FullPiece();

    // Constructor from members
    FullPiece(const QVector<char> & piece);

    // Constructor based on raw payload
    FullPiece(QDataStream & stream, int lengthOfPiece);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    QVector<char> piece() const;
    void setPiece(const QVector<char> & piece);

private:

    // No need for piece index!

    // Piece data;
    QVector<char> _piece;

};

#endif // FULLPIECE_HPP
