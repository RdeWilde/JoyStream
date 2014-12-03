
#ifndef CONFIG_HPP
#define CONFIG_HPP

#define CLIENT_FINGERPRINT "BM" // Bedeho Mender, was almost no other ones left (https://wiki.theory.org/BitTorrentSpecification)

#define BITSWAPR_VERSION_MAJOR 0
#define BITSWAPR_VERSION_MINOR 1

// Time between each time session.pop_alerts() is called, in milliseconds
//#define CONTROLLER_POP_ALERTS_DELAY 200

// Time between each time session.post_torrent_updates() is called, in milliseconds
#define POST_TORRENT_UPDATES_DELAY 1000

#define PARAMETER_FILE_NAME "parameters.txt"

#define PAYMENT_PROTOCOL_VERSION 1

#define EXTENSION_NAME "bm_payment" // extension name

#define APPLICATION_NAME "BitSwapr"

#define APPLICATION_DESCRIPTION "A cryptocurrency enabled BitTorrent client."


#endif
