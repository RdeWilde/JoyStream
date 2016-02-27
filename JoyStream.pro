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
            #plugin \
            core \
            views \
            app \
            runner \
            test \
            demo \
            bitcoin

# paymentchannel
paymentchannel.depends = common

# blockcypher
blockcypher.depends = common

# plugin
#plugin.depends = common
#plugin.depends = blockcypher

# core
core.depends = common
core.depends = paymentchannel
#core.depends = plugin

# views
views.depends = common
#views.depends = plugin
views.depends = core

# app
app.depends = mixpanel
app.depends = common
app.depends = core
#app.depends = plugin
app.depends = views
app.depends = bitcoin
app.depends = blockcypher

# runner
runner.depends = common
runner.depends = core
#runner.depends = plugin
#runner.depends = views

# test
test.depends = common
test.depends = paymentchannel
test.depends = blockcypher
#test.depends = plugin
test.depends = core
#test.depends = views
#test.depends = app
#test.depends = runner

# demo
demo.depends = common
demo.depends = paymentchannel
demo.depends = blockcypher
#demo.depends = plugin
demo.depends = core
demo.depends = views
#demo.depends = app
demo.depends = runner
