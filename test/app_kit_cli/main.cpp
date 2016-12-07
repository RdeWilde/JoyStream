/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <CliApp.hpp>

#include <boost/asio/impl/src.hpp>

int main(int argc, char *argv[])
{
    if(getenv("DEBUG") != NULL) {
        INIT_LOGGER("netsync.log");
    }

    auto env_data_dir = getenv("JOYSTREAM_DATADIR");
    std::string kitDataDirectory =  env_data_dir != NULL ? env_data_dir : QDir::currentPath().toStdString();

    CliApp app(argc, argv, kitDataDirectory, Coin::Network::testnet3);
    int ret = app.run();
    return ret;
}
