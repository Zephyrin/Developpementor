TEMPLATE = app
CONFIG += console
CONFIG += app_bundle
CONFIG += c++11
QT += network
QT += core
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET="view"

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

# Simulation #
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../simulation/release/ -lsimulation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../simulation/debug/ -lsimulation
else:symbian: LIBS += -lsimulation
else:unix: LIBS += -L$$OUT_PWD/../simulation/ -lsimulation

INCLUDEPATH += $$PWD/../simulation
DEPENDPATH += $$PWD/../simulation

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../simulation/release/libsimulation.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../simulation/debug/libsimulation.a

# GraphicsView #
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../graphicsview/release/ -lgraphicsview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../graphicsview/debug/ -lgraphicsview
else:symbian: LIBS += -lgraphicsview
else:unix: LIBS += -L$$OUT_PWD/../graphicsview/ -lgraphicsview

INCLUDEPATH += $$PWD/../graphicsview
DEPENDPATH += $$PWD/../graphicsview

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicsview/release/libgraphicsview.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../graphicsview/debug/libgraphicsview.a
#else:unix:!symbian: PRE_TARGETDEPS += $$OUT_PWD/../graphicsview/libgraphicsview.a

ANDROID_EXTRA_LIBS=

SOURCES += main.cpp \
    mainwindow.cpp \

HEADERS += \
    mainwindow.h \

FORMS += \
    mainwindow.ui \

DISTFILES +=
