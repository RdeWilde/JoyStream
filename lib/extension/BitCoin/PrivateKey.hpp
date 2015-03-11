#ifndef PRIVATE_KEY_HPP
#define PRIVATE_KEY_HPP

#include "FixedBuffer.hpp"

// strlen(KzVEsf4fzZPk9twhdSP2LviTLjBjqKDNLZSGVPRkchNKZFkkigum) = 52 hex = 26 bytes
typedef FixedBuffer<52> PrivateKey;

#endif // PRIVATE_KEY_HPP
