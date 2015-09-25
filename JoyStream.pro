# Copyright (C) JoyStream - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015

TEMPLATE    = subdirs

SUBDIRS     = \
            common \
            paymentchannel \
            blockcypher \
            wallet \
            plugin \
            core \
            views \
            app \
            runner \
            test \
            demo

# paymentchannel
paymentchannel.depends = common

# blockcypher
blockcypher.depends = common

# wallet
wallet.depends = common
wallet.depends = blockcypher
# wallet.depends = paymentchannel # not yet

# plugin
plugin.depends = common
plugin.depends = blockcypher
plugin.depends = wallet

# core
core.depends = common
core.depends = paymentchannel
core.depends = wallet
core.depends = plugin

# views
views.depends = common
views.depends = wallet
views.depends = plugin
views.depends = core

# app
app.depends = common
app.depends = core
app.depends = wallet
app.depends = plugin
app.depends = views

# runner
runner.depends = common
runner.depends = core
runner.depends = wallet
runner.depends = plugin
runner.depends = views

# test
test.depends = common
test.depends = paymentchannel
test.depends = blockcypher
test.depends = wallet
test.depends = plugin
test.depends = core
test.depends = views
#test.depends = app
test.depends = runner

# demo
demo.depends = common
demo.depends = paymentchannel
demo.depends = blockcypher
demo.depends = wallet
demo.depends = plugin
demo.depends = core
demo.depends = views
#demo.depends = app
demo.depends = runner
