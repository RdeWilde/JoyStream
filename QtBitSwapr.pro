
TEMPLATE    = subdirs
SUBDIRS     = \
            lib \
            app \
            tests

app.depends = lib
tests.depends = lib
