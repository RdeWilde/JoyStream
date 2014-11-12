
TEMPLATE    = subdirs
SUBDIRS = \
    lib \
    app \
    test
app.depends = lib
tests.depends = lib
