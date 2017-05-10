#-------------------------------------------------
#
# Project created by QtCreator 2014-12-24T19:04:37
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += serialport
QT       -= gui

TARGET = UdpUartBridge
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    packetinterface.cpp \
    udpserver.cpp \
    usbport.cpp

HEADERS += \
    packetinterface.h \
    serialport.h \
    udpserver.h \
    usbport.h


win32: LIBS += -L$$PWD/import/libs/ -llibusb-1.0

INCLUDEPATH += $$PWD/import/inc
DEPENDPATH += $$PWD/import/inc

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/import/libs/libusb-1.0.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/import/libs/libusb-1.0.a
