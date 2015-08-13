/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>


/**
#include <core/Config.hpp>
#include <core/ControllerTracker.hpp>
#include <core/controller/Controller.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp> // for configurations
#include <core/extension/SellerTorrentPlugin.hpp> // for configurations
#include <core/logger/LoggerManager.hpp>
#include <core/extension/PluginMode.hpp>
*/

//#include <common/Bitcoin.hpp> // defines STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE
//#include <common/UnspentP2PKHOutput.hpp>
//#include <common/BitcoinRepresentation.hpp>

#include <openssl/ripemd.h>

//#include <common/FixedUCharArray.hpp>

//#include <common/TransactionId.hpp>

//#include <common/UnspentP2PKHOutput.hpp>

#include <common/UnspentP2PKHOutput.hpp>

// JoyStream entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);



/**
    unsigned char digest[RIPEMD160_DIGEST_LENGTH];
    char string[] = "hello world";

    RIPEMD160((unsigned char*)&string, strlen(string), (unsigned char*)&digest);

    char mdString[RIPEMD160_DIGEST_LENGTH*2+1];

    for(int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
         sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("RIPEMD160 digest: %s\n", mdString);
*/


    //Coin::FixedUCharArray<22> test;

    Coin::UnspentP2PKHOutput x;

    app.exec();

    qDebug() << "Application event loop exited, application closing.";

    return 0;
}
