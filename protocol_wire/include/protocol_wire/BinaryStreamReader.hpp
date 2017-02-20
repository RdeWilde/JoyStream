#ifndef JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMREADER_HPP
#define JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMREADER_HPP

#include <protocol_wire/char_array_buffer.hpp>
#include <protocol_wire/NetworkInt.hpp>

namespace joystream {
namespace protocol_wire {

class BinaryStreamReader {
public:

    // Construct the stream from streambuf
    BinaryStreamReader(std::streambuf* buf);

    // read integers
    template<class IntType>
    IntType readInt();

    std::streamsize readBytes(unsigned char *destination, std::streamsize size);
    std::streamsize readBytes(char* destination, std::streamsize size);

    // read size bytes into the vector starting at beginning,
    // vector size will be resized to size
    std::streamsize readBytes(std::vector<unsigned char> & destination, std::streamsize size);

private:
    // prevent copy and assignment ?

    // stream buffer we will write to
    std::streambuf* _buffer;
};

template<class IntType>
IntType BinaryStreamReader::readInt() {
    const auto size = NetworkInt<IntType>::size();

    char data[size];

    auto read = readBytes(data, size);

    if(read != size)
        throw std::runtime_error("reading from stream buffer failed");

    return NetworkInt<IntType>(data).value();
}

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMREADER_HPP
