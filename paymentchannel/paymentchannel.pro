# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015

TEMPLATE    = subdirs

SUBDIRS     = \
            lib \
	    test


test.depends = lib
