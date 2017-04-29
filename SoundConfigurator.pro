#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T01:28:00
#
#-------------------------------------------------

QT       += core gui multimedia winextras serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia winextras

TARGET = SoundConfigurator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    src/mainwindow.cpp \
    src/ACCESSDATE/accessdata.cpp \
    src/FORM/createdayplan.cpp \
    src/FORM/createholidays.cpp \
    src/PLAYER/musicplayer.cpp \
    src/PLAYER/volumebutton.cpp \
    src/FORM/createformplaylist.cpp \
    src/INTERFACE/settingsdialog.cpp \
    src/INTERFACE/settingdialogwifi.cpp \
    src/INTERFACE/comport.cpp \
    src/INTERFACE/wifi.cpp

HEADERS  += src/mainwindow.h \
    src/ACCESSDATE/accessdata.h \
    src/FORM/createdayplan.h \
    src/FORM/createholidays.h \
    src/FORM/InterfaceForms.h \
    src/PLAYER/musicplayer.h \
    src/PLAYER/volumebutton.h \
    src/FORM/createformplaylist.h \
    src/INTERFACE/LinkInterface.h \
    src/INTERFACE/settingsdialog.h \
    src/INTERFACE/settingdialogwifi.h \
    src/INTERFACE/comport.h \
    src/INTERFACE/wifi.h

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    settingdialogwifi.ui

SUBDIRS += \
    configurator.pro

RESOURCES += \
    Resources.qrc
QMAKE_CXXFLAGS += -std=c++0x
