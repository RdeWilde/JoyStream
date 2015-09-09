# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

TEMPLATE    = subdirs

SUBDIRS     = \
            common \
            paymentchannel \  	
            wallet \
            core \
            views \
            app \
            blockcypher \
            test

paymentchannel.depends = common

wallet.depends = common
# wallet.depends = paymentchannel

core.depends = common
core.depends = paymentchannel
core.depends = wallet

views.depends = common
views.depends = wallet
views.depends = core

app.depends = common
app.depends = core
app.depends = wallet
app.depends = views

test.depends = common
test.depends = paymentchannel
test.depends = wallet
test.depends = core
test.depends = views
test.depends = app
test.depends = blockcypher

