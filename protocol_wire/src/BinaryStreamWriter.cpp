#include <protocol_wire/BinaryStreamWriter.hpp>

namespace joystream {
namespace protocol_wire {

BinaryStreamWriter::BinaryStreamWriter(std::streambuf* buf)
    : _buffer(buf) {
}

std::streamsize BinaryStreamWriter::writeBytes(const std::vector<unsigned char> & from) {
    if(_buffer == nullptr) return from.size();

    return writeBytes(from.data(), from.size());
}

std::streamsize BinaryStreamWriter::writeBytes(const unsigned char* from, std::streamsize size) {

    if(_buffer == nullptr) return size;

    auto ptr = reinterpret_cast<const char*>(from);

    auto written = _buffer->sputn(ptr, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}

std::streamsize BinaryStreamWriter::writeBytes(const char* from, std::streamsize size) {
    if(_buffer == nullptr) return size;

    auto written = _buffer->sputn(from, size);

    if(written != size) {
        throw std::runtime_error("stream buffer full, writing failed");
    }

    return written;
}

}
}
