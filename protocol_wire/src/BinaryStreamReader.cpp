#include <protocol_wire/BinaryStreamReader.hpp>
#include <protocol_wire/NetworkInt.hpp>

namespace joystream {
namespace protocol_wire {

BinaryStreamReader::BinaryStreamReader(std::streambuf *buf)
    : _buffer(buf) {
}

std::streamsize BinaryStreamReader::readBytes(unsigned char* data, std::streamsize size) {
    auto ptr = reinterpret_cast<char*>(data);

    auto read = _buffer->sgetn(ptr, size);

    if(read != size) {
        throw std::runtime_error("error reading from stream buffer");
    }

    return read;
}

std::streamsize BinaryStreamReader::readBytes(char *data, std::streamsize size) {
    auto read = _buffer->sgetn(data, size);;

    if(read != size) {
        throw std::runtime_error("error reading from stream buffer");
    }

    return read;
}

std::streamsize BinaryStreamReader::readBytes(std::vector<unsigned char> & data, std::streamsize size) {
    data.resize(size);
    return readBytes(data.data(), size);
}

}
}
