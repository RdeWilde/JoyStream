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
            protocol_wire \
            protocol_statemachine \
            protocol_session \
            core \
            views \
            app \
            runner \
            test \
            demo

# app
app.depends = mixpanel
app.depends = common
app.depends = core
app.depends = wallet
app.depends = views
app.depends = extension
app.depends = protocol_session
app.depends = protocol_statemachine
app.depends = protocol_wire
app.depends = blockcypher
app.depends = paymentchannel

# runner
runner.depends = common
runner.depends = core
runner.depends = wallet
runner.depends = views

# test
test.depends = mixpanel
test.depends = common
test.depends = core
test.depends = wallet
test.depends = views
test.depends = extension
test.depends = protocol_session
test.depends = protocol_statemachine
test.depends = protocol_wire
test.depends = blockcypher
test.depends = paymentchannel


# demo
demo.depends = mixpanel
demo.depends = common
demo.depends = core
demo.depends = wallet
demo.depends = views
demo.depends = extension
demo.depends = protocol_session
demo.depends = protocol_statemachine
demo.depends = protocol_wire
demo.depends = blockcypher
demo.depends = paymentchannel
