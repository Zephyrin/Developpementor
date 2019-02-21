#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T11:54:28
#
#-------------------------------------------------

QT       -= core gui
CONFIG += c++11
TARGET = agent
TEMPLATE = lib

DEFINES += AGENT_LIBRARY

SOURCES += \
    environment.cpp \
    agent.cpp \
    hedonism.cpp \
    stooge.cpp \
    anticipation.cpp \
    interaction.cpp \
    interactions.cpp \
    phenomenas.cpp \
    trace.cpp \
    phenomenal.cpp \
    existencehedonism.cpp \
    existencephenomenal.cpp \
    unknownbelieve.cpp \
    minimumalgorithm.cpp \
    existenceminimumalgorithm.cpp \
    signatures.cpp \
    signature.cpp \
    usagetable.cpp \
    obsel.cpp

HEADERS += \
    environment.hpp \
    agent.hpp \
    hedonism.hpp \
    stooge.hpp \
    anticipation.hpp \
    interaction.hpp \
    interactions.hpp \
    mood.hpp \
    phenomenas.hpp \
    trace.hpp \
    phenomenal.hpp \
    existencehedonism.hpp \
    existencephenomenal.hpp \
    unknownbelieve.h \
    existenceminimumalgorithm.hpp \
    minimumalgorithm.hpp \
    signatures.hpp \
    signature.hpp \
    usagetable.hpp \
    obsel.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=
