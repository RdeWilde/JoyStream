#include "Utilities.hpp"

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp> // bdecode()

#include <fstream>
#include <vector>

bool Utilities::loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry) {

    // Open file at the end, so we can get size
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ifstream::ate);

    // Get size of file
    int sizeOfFile = (int)file.tellg();

    // Rewind position
    file.seekg(0);

    // Allocate space for raw file
    std::vector<char> dataVector(sizeOfFile, 0);

    // Read entire file into vector
    file.read(&dataVector[0], dataVector.size());

    // Close file
    file.close();

    // Check that entire file was read,
    // if not, indicate failure
    if(!file)
        return false;

    // Bendecode entry
    destinationEntry = libtorrent::bdecode(dataVector.begin(), dataVector.end());

    // Indicate success
    return true;
}

bool Utilities::saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry) {

    // Bencode entry
    std::vector<char> bencodedEntry;
    bencode(std::back_inserter(bencodedEntry), sourceEntry);

    // Open file, overwrite if present
    std::ofstream file(fileName, std::ios::out | std::ios::binary | std::ios::trunc);

    // Save to file
    file.write(reinterpret_cast<const char*>(&bencodedEntry[0]), bencodedEntry.size());

    // Check that entire file was written
    // if not, indicate failure
    if(!file)
        return false;

    // Close file
    file.close();

    // Indicate success
    return true;
}
