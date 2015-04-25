#ifndef _QUADRO_WEBVIEW_HPP
#define _QUADRO_WEBVIEW_HPP

#include <QContextMenuEvent>
#include <QWebView>

namespace Quadro {

class WebView
  : public QWebView
{
  Q_OBJECT

  public:
    WebView(void)
    {
      // create & set transparent palette for browser window
      QPalette palette = this->palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      this->page()->setPalette(palette);

      // enable transparency for underlying window
      this->setAttribute(Qt::WA_TranslucentBackground, true);
    }

  protected:
    void
    contextMenuEvent(QContextMenuEvent * e)
    {
      emit contextMenuSignal(e, this);
    }

  signals:
    void contextMenuSignal(QContextMenuEvent * event, WebView * webview);
};

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_HPP
