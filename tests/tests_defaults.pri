
# Some common stuff across all tests

# Link with lib
# conditions required because on windows the builds are put in extra release/debug subfolders
win32:CONFIG(release, debug|release):       LIBS += -L$$OUT_PWD/../../lib/release/ -lQtBitSwapr
else:win32:CONFIG(debug, debug|release):    LIBS += -L$$OUT_PWD/../../lib/debug/ -lQtBitSwapr
else:unix:                                  LIBS += -L$$OUT_PWD/../../lib/ -lQtBitSwapr














