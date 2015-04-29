#ifndef _QUADRO_WEBVIEW_HPP
#define _QUADRO_WEBVIEW_HPP

#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QWebView>
#include <QX11Info>

#include "Ui.hpp"
#include "Config.hpp"

namespace Quadro {

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
    resizeEvent(QResizeEvent * e);

    void
    contextMenuEvent(QContextMenuEvent * e);

  private:
    Ui m_Ui;
};

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_HPP
