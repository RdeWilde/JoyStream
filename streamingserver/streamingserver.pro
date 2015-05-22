
TEMPLATE    = subdirs
SUBDIRS     = \
            streamingserver-lib \
            streamingserver-test

streamingserver-test.depends = streamingserver-lib
