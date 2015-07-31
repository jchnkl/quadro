#ifndef _QUADRO_WEBVIEW_HPP
#define _QUADRO_WEBVIEW_HPP

#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QWebView>
#include <QX11Info>

#include "Ui.hpp"
#include "Config.hpp"
#include "DBusBridge.hpp"
#include "FileBridge.hpp"
#include "SystemBridge.hpp"

namespace Quadro {

class WebPage
  : public QWebPage
{
  Q_OBJECT
  protected:
    void javaScriptConsoleMessage(const QString & message,
                                  int lineNumber,
                                  const QString & sourceID);
}; // class WebPage

class WebView
  : public QWebView
{
  Q_OBJECT

  signals:
    void contextMenuSignal(QContextMenuEvent * event, WebView * webview);

  public slots:
    void
    onLoadUrl(const QString & url);

  public:
    WebView(const Config & config);

  protected:
    void
    onMoveBy(const QPoint & offset);

    void
    onResizeBy(const QMargins & offset);

    void
    contextMenuEvent(QContextMenuEvent * e);

    void
    onJsWindowObjectCleared(void);

    QScreen *
    getScreen(void);

    void
    changeNetWmWindowType(NetWmWindowType::Hint);

  private:
    Config m_Config;
    DBus m_DBus;
    File m_File;
    Stat m_Stat;
    System m_System;
    WebPage m_WebPage;
};

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_HPP
