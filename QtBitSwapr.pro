
TEMPLATE    = subdirs
SUBDIRS     = \
            lib \
            app \
            streamingserver \
            tests

lib.depends = streamingserver # does this mean it depends on test as well?
app.depends = lib
tests.depends = lib
