
TEMPLATE    = subdirs
SUBDIRS     = \
            common \
            lib \
            app \
            tests

lib.depends = common

app.depends = common
app.depends = lib

test.depends = common
tests.depends = lib
