TEMPLATE = lib
CONFIG -= console
CONFIG += app_bundle
CONFIG += c++11
QT += core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET=simulation
DEFINES += SIMULATION_LIBRARY
# lib Agent #
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../agent/release/ -lagent
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../agent/debug/ -lagent
else:symbian: LIBS += -lagent
else:unix: LIBS += -L$$OUT_PWD/../agent/ -lagent

INCLUDEPATH += $$PWD/../agent
DEPENDPATH += $$PWD/../agent

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../agent/release/libagent.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../agent/debug/libagent.a
#else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../agent/libagent.a

# Ktbs #
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ktbs/release/ -lktbs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ktbs/debug/ -lktbs
else:symbian: LIBS += -lktbs
else:unix: LIBS += -L$$OUT_PWD/../ktbs/ -lktbs

INCLUDEPATH += $$PWD/../ktbs
DEPENDPATH += $$PWD/../ktbs

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ktbs/release/libktbs.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../ktbs/debug/libktbs.a
#else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../ktbs/libktbs.a

SOURCES += \
    environment1.cpp \
    environment2.cpp \
    environment4.cpp \
    environmentsmallloop.cpp \
    environmentstring.cpp \
    environmentstring2.cpp \
    simulation.cpp

HEADERS += \
    environment1.h \
    environment2.hpp \
    environment4.h \
    environmentsmallloop.h \
    environmentstring.h \
    environmentstring2.h \
    simulation.h

RESOURCES += \
    simulation.qrc


