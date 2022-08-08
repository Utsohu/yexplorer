#-------------------------------------------------
#
# Project created by QtCreator 2022-02-27T15:32:34
#
#-------------------------------------------------

QT       += core gui webenginewidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yexplore
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
        addfavoritedialog.cpp \
        appinfo.cpp \
        cfavorite.cpp \
        main.cpp \
        newdirdialog.cpp \
        webcontent.cpp \
        webpage.cpp \
        ywindow.cpp

HEADERS += ywindow.h \
    addfavoritedialog.h \
    appinfo.h \
    cfavorite.h \
    newdirdialog.h \
    webcontent.h \
    webpage.h

FORMS += \
        addfavoritedialog.ui \
        appinfo.ui \
        newdirdialog.ui \
        webpage.ui \
        ywindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
RC_ICONS = yexplore.ico
