#-------------------------------------------------
#
# Project created by QtCreator 2020-07-07T06:18:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FSHK-AP
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        fshkwindow.cpp \
    classlehrling.cpp \
    classbetrieb.cpp \
    formbetrieb.cpp \
    formlehrling.cpp \
    formbetriebliste.cpp \
    classfrage.cpp \
    classprojekt.cpp \
    formprojekt.cpp \
    classskills.cpp \
    formskills.cpp \
    formzuordnen.cpp \
    formupdateinformation.cpp \
    formevaluation.cpp

HEADERS += \
        fshkwindow.h \
    classlehrling.h \
    classbetrieb.h \
    formbetrieb.h \
    formlehrling.h \
    formbetriebliste.h \
    classfrage.h \
    classprojekt.h \
    formprojekt.h \
    classskills.h \
    formskills.h \
    formzuordnen.h \
    formupdateinformation.h \
    formevaluation.h

FORMS += \
        fshkwindow.ui \
    formbetrieb.ui \
    formlehrling.ui \
    formbetriebliste.ui \
    formprojekt.ui \
    formskills.ui \
    formzuordnen.ui \
    formupdateinformation.ui \
    formevaluation.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
