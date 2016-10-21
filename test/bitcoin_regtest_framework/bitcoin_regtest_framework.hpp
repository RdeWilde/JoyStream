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

std::string make_cli_command(const std::string & command) {
    std::string sys_string = "bitcoin-cli -regtest -rpcuser=test -rpcpassword=test ";
    return sys_string + command;
}

int rpc(const std::string &command) {
    return system(make_cli_command(command).c_str());
}

int rpc_output(std::string command, std::string & result) {
    FILE *output = popen(make_cli_command(command).c_str(), "r");

    if (!output) {
        return errno;
    }

    char buffer[64000];
    char *line_p;
    while((line_p = fgets(buffer, sizeof(buffer), output))) {
        result += buffer;
    }

    // strip off trailing new line character
    if(result.back() == '\n') {
        result.resize(result.size() - 1);
    }

    return pclose(output);
}

int wait_for_ready() {
    return rpc("-rpcwait getinfo");
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int generate_blocks(const uint & number_of_blocks) {
    std::stringstream command;
    command << "generate ";
    command << number_of_blocks;
    return rpc(command.str());
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int send_to_address(const std::string &addr, const std::string &amount) {
    std::stringstream command;
    command << "sendtoaddress ";
    command << addr << " ";
    command << amount;
    return rpc(command.str());
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int send_to_address(const std::string &addr, const std::string &amount, std::string &txid) {
    std::stringstream command;
    command << "sendtoaddress ";
    command << addr << " ";
    command << amount;
    return rpc_output(command.str(), txid);
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int send_raw_transaction(const std::string &rawSignedTransaction) {
    return rpc("sendrawtransaction " + rawSignedTransaction);
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT std::string getnewaddress() {
    std::string newAddress;
    int err = rpc_output("getnewaddress", newAddress);

    if(err)
        throw std::runtime_error(std::string("bitcoin: unable to generate a new address") + std::string(strerror(err)));

    return newAddress;
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT std::string getPrivKey(std::string base58EncodedAddress) {
    std::string privKey;
    int err = rpc_output("dumpprivkey " + base58EncodedAddress, privKey);

    if(err)
        throw std::runtime_error(std::string("bitcoin: unable to get private key") + std::string(strerror(err)));

    return privKey;
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT int importPrivKey(std::string privKey) {
    return rpc("importprivkey " + privKey);
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT std::string listunspent() {
    std::string list;
    int err = rpc_output("listunspent 0", list);

    if(err)
        throw std::runtime_error(std::string("bitcoin: unable to get list of unspent outputs") + std::string(strerror(err)));

    return list;
}

BITCOIN_REGTEST_FRAMEWORKSHARED_EXPORT std::string getrawtransaction(std::string txId) {
    std::string rawtx;
    int err = rpc_output("getrawtransaction " + txId, rawtx);

    if(err)
        throw std::runtime_error(std::string("bitcoin: unable to get raw transaction") + std::string(strerror(err)));

    return rawtx;
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
