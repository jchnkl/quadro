#ifndef _QUADRO_WEBVIEW_HPP
#define _QUADRO_WEBVIEW_HPP

#include <QContextMenuEvent>
#include <QWebView>

namespace Browser {

class WebView
  : public QWebView
{
  Q_OBJECT

  protected:
    void
    contextMenuEvent(QContextMenuEvent * e)
    {
      emit contextMenuSignal(e, this);
    }

  signals:
    void contextMenuSignal(QContextMenuEvent * event, WebView * webview);
};

}; // namespace Browser

#endif // _QUADRO_WEBVIEW_HPP
