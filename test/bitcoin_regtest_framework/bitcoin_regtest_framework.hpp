#ifndef BITCOIN_REGTEST_FRAMEWORK_HPP
#define BITCOIN_REGTEST_FRAMEWORK_HPP

#include "bitcoin_regtest_framework_global.hpp"
#include <stdlib.h>
#include <string>
#include <sstream>

namespace joystream {
namespace test {
namespace bitcoin {
namespace regtest {

/*
 *  path to bitcoind and bitcoin-cli need to be
 *  added to the Qt project 'Run Environment' PATH variable
 */

// Helper Functions for starting stopping and issuing commands to bitcoind
int create_data_dir() {
    return system("mkdir -p bitcoind-test/");
}

int purge_data_dir() {
    return system("rm -fr bitcoind-test/");
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int rpc(const std::string & command) {
    std::string sys_string = "bitcoin-cli -regtest -rpcuser=test -rpcpassword=test ";
    std::string full_commandline = sys_string + command;

    return system(full_commandline.c_str());
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int rpc(const std::stringstream & command) {
    return rpc(command.str());
}

int wait_for_ready() {
    return rpc("-rpcwait getinfo");
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int generate_blocks(const uint & number_of_blocks) {
    std::stringstream command;
    command << "generate ";
    command << number_of_blocks;
    return rpc(command);
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int send_to_address(const std::string &addr, const std::string &amount) {
    std::stringstream command;
    command << "sendtoaddress ";
    command << addr << " ";
    command << amount;
    return rpc(command);
}

int start_node() {
    return system("bitcoind -regtest -daemon -datadir=bitcoind-test/ -rpcuser=test -rpcpassword=test");
}

int stop_node() {
    return rpc("stop");
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int init() {
    // Stop the running regtest node
    stop_node();

    // Reset regtest blockchain
    purge_data_dir();

    int result;

    // Prepare local temporary directory for bitcoind
    result = create_data_dir();

    if(result != 0) return result;

    // Start bitcoind daemon
    result = start_node();

    if(result != 0) return result;

    // Wait for it to start
    result = wait_for_ready();

    if(result != 0) return result;

    // mine first 101 blocks to make first coinbase tx spendable
    result = generate_blocks(101);

    return result;
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT void shutdown() {
    stop_node();
}

}}}}

#endif // BITCOIN_REGTEST_FRAMEWORK_HPP
