QT += core gui webkitwidgets x11extras

CONFIG += c++11 link_pkgconfig
PKGCONFIG += xcb xcb-ewmh

greaterThan(QT_MAFOR_VERSION, 4): QT += widets

TARGET = quadro
TEMPLATE = app

HEADERS += WebView.hpp Ui.hpp Config.hpp Util.hpp Ewmh.hpp NetWmWindowType.hpp
SOURCES += Main.cpp Ui.cpp Util.cpp
