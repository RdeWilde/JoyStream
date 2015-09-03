
#include <common/SigHashType.hpp>

#include <stdexcept>

namespace Coin {

unsigned char valueForSighashType(SigHashType type) {

    switch(type) {

        case SigHashType::all: return 0x01;
        case SigHashType::none: return 0x02;
        case SigHashType::single: return 0x03;
        case SigHashType::anyonecanpay: return 0x80;

        default:
            throw std::runtime_error("Coding error, unsupported SigHashType");
    }
}

SigHashType SigHashTypeFromValue(unsigned char value) {

    switch(value) {

        case 0x01: return SigHashType::all;
        case 0x02: return SigHashType::none;
        case 0x03: return SigHashType::single;
        case 0x80: return SigHashType::anyonecanpay;

        default:
            throw std::runtime_error("Invalid sighashtype value");
    }
}

}
