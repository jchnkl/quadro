QT += core gui webkitwidgets x11extras

CONFIG += link_pkgconfig
PKGCONFIG += xcb

greaterThan(QT_MAFOR_VERSION, 4): QT += widets

TARGET = salsa
TEMPLATE = app

SOURCES += main.cpp
