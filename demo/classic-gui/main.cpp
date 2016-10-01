/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */


#include "ClassicGUIDemoDialog.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    joystream::demo::ClassicGUIDemoDialog dialog;
    dialog.show();

    QObject::connect(&dialog,
                     &joystream::demo::ClassicGUIDemoDialog::finished,
                     &a,
                     &QApplication::quit);

    return a.exec();
}
