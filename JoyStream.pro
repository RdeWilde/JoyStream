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
            wallet \
            extension \
            protocol \
            wire \
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

# wire
wire.depends = common

# protocol
protocol.depends = common
protocol.depends = wire
protocol.depends = paymentchannel

# wallet
wallet.depends = common
wallet.depends = blockcypher
wallet.depends = paymentchannel # not yet ?

# extension
extension.depends = common
extension.depends = paymentchannel
extension.depends = protocol

# core
core.depends = common
core.depends = paymentchannel
core.depends = wallet
core.depends = extension

# views
views.depends = common
views.depends = wallet
views.depends = core

# app
app.depends = mixpanel
app.depends = common
app.depends = core
app.depends = wallet
app.depends = views

# runner
runner.depends = common
runner.depends = core
runner.depends = wallet
runner.depends = views

# test
test.depends = common
test.depends = paymentchannel
test.depends = blockcypher
test.depends = wallet
test.depends = extension
test.depends = core
test.depends = views
test.depends = runner

# demo
demo.depends = common
demo.depends = paymentchannel
demo.depends = blockcypher
demo.depends = wallet
demo.depends = extension
demo.depends = core
demo.depends = views
demo.depends = runner
