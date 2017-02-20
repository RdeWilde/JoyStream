#ifndef JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMWRITER_HPP
#define JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMWRITER_HPP

#include <protocol_wire/char_array_buffer.hpp>
#include <protocol_wire/NetworkInt.hpp>

namespace joystream {
namespace protocol_wire {

class BinaryStreamWriter {
public:

    // Construct the stream from streambuf
    BinaryStreamWriter(std::streambuf* buf);

    // write integers
    template<class IntType>
    std::streamsize writeInt(IntType value);

    // write raw data
    std::streamsize writeBytes(const unsigned char *, std::streamsize);
    std::streamsize writeBytes(const char *, std::streamsize);
    std::streamsize writeBytes(const std::vector<unsigned char>&);

private:
    // prevent copy and assignment ?

    // stream buffer we will write to
    std::streambuf* _buffer;
};

template<class IntType>
std::streamsize BinaryStreamWriter::writeInt(IntType value) {
    NetworkInt<IntType> encodedInt(value);

    return writeBytes(encodedInt.data(), encodedInt.rawLength());
}

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_BINARYSTREAMWRITER_HPP
