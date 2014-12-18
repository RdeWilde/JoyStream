#ifndef UTILITIES_HPP
#define UTILITIES_HPP

namespace libtorrent {
    class entry;
}

/**
 * Static utility functions
 */
class Utilities
{
public:

    // Opens file, reads contents, debencodes into destinationEntry
    static bool loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry);

    // Bencodes dictionary, opens file, writes to file
    static bool saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry);
};

#endif // UTILITIES_HPP
