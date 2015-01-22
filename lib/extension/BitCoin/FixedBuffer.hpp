#ifndef FIXED_BUFFER_HPP
#define FIXED_BUFFER_HPP

class QDataStream;

/**
 * Paremetrized buffer w.r.t integer
 */
template <unsigned int L>
class FixedBuffer
{
public:

    static const quint32 length = L;

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    FixedBuffer();
    FixedBuffer(const FixedBuffer<L>& fixedbuffer);
    FixedBuffer & operator=(const FixedBuffer<L>& rhs);

    /**
     * Utilities
     */
    char at(unsigned int index) const;
    void set(unsigned int index, char value);
    void clear();

    /**
     * Iterator routines.
     */
    const char * begin() const;
    const char * end() const;

    char * begin();
    char * end();

    /**
     * Operator overloading.
     */
    friend QDataStream & operator<<(QDataStream& stream, const FixedBuffer<L> & fixedBuffer);
    friend QDataStream & operator>>(QDataStream& stream, FixedBuffer<L> & fixedBuffer);

private:

    // Actual type fixed buffer
    char _buffer[L];
};

#endif // FIXED_BUFFER_HPP
