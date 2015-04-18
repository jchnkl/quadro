QT += core gui webkitwidgets x11extras

CONFIG += c++11 link_pkgconfig
PKGCONFIG += xcb xcb-ewmh

greaterThan(QT_MAFOR_VERSION, 4): QT += widets

TARGET = salsa
TEMPLATE = app

SOURCES += main.cpp
