# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

TEMPLATE    = subdirs

SUBDIRS     = \
            mixpanel \
            common \
            paymentchannel \
            blockcypher \
            bitcoin \
            core \
            views \
            app \
            runner \
            test \
            demo \

# ensure that the subdirectories are built in the order in which they are specified
CONFIG += ordered
