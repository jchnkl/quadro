QT += core dbus gui webkitwidgets x11extras

CONFIG += c++11 link_pkgconfig
PKGCONFIG += xcb xcb-ewmh

TARGET = quadro
TEMPLATE = app

HEADERS += WebView.hpp Ui.hpp Config.hpp Util.hpp DBusBridge.hpp FileBridge.hpp Ewmh.hpp NetWmState.hpp NetWmWindowType.hpp
SOURCES += Main.cpp WebView.cpp Ui.cpp Config.cpp Util.cpp DBusBridge.cpp FileBridge.cpp
