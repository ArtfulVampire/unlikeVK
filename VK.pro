QT += core
QT -= gui



CONFIG += c++14
QT += network

TARGET = VK
CONFIG -= console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lib.cpp

HEADERS += \
    lib.h
