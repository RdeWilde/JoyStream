# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

TEMPLATE    = subdirs

SUBDIRS     = \
            common \
            wallet \
            core \
            views \
            app

wallet.depends = common

core.depends = common
core.depends = wallet

views.depends = common
views.depends = wallet
views.depends = core

app.depends = common
app.depends = core
app.depends = wallet
app.depends = views
