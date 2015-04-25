#ifndef _QUADRO_WEBVIEW_HPP
#define _QUADRO_WEBVIEW_HPP


#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QMenu>
#include <QWebView>
#include <QX11Info>

#include "Ui.hpp"
#include "Config.hpp"
#include "NetWmWindowType.hpp"

namespace Quadro {

class WebView
  : public QWebView
{
  Q_OBJECT

  signals:
    void contextMenuSignal(QContextMenuEvent * event, WebView * webview);

  public slots:
    void
    onLoadUrl(const QString & url)
    {
      this->load(QUrl::fromUserInput(url));
    }

  public:
    WebView(const Config & config)
      : m_Ui(this)
    {
      // create & set transparent palette for browser window
      QPalette palette = this->palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      this->page()->setPalette(palette);

      // set object name for style sheet
      this->setObjectName("QuadroWebView");

      // remove the border
      this->setStyleSheet("QGroupBox#QuadroWebView { border: 0px; }");

      // hint for window manager to leave away frames/window borders
      this->setWindowFlags(Qt::FramelessWindowHint);

      // make window background transparent
      this->setAttribute(Qt::WA_TranslucentBackground, true);

      Ewmh ewmh(QX11Info::connection());
      NetWmWindowType windowType(ewmh, this->winId());

      windowType.clear(NetWmWindowType::Same).add(config.windowTypeHint());

      connect(&m_Ui, &Ui::loadUrl, this, &WebView::onLoadUrl);
      connect(this, &QWebView::urlChanged, &m_Ui, &Ui::onUrlChanged);

      this->setGeometry(config.x(), config.y(), config.width(), config.height());

      if (config.hasUrl()) {
        this->load(QUrl::fromUserInput(config.url()));
      }

      if (config.fullscreen()) {
        QRect desktop_rect = QApplication::desktop()->screenGeometry();
        this->setGeometry(desktop_rect);
      }

      this->show();
    }

  protected:
    void
    resizeEvent(QResizeEvent * e)
    {
      m_Ui.resize(e->size());
      QWebView::resizeEvent(e);
    }

    void
    contextMenuEvent(QContextMenuEvent * e)
    {
      QMenu * menu = this->page()->createStandardContextMenu();

      QAction showUiAction(menu->addSeparator());;

      if (menu->isEmpty()) {
        menu->addAction(&showUiAction);
      } else {
        QAction * first = menu->actions().first();
        menu->insertAction(menu->insertSeparator(first), &showUiAction);
      }

      showUiAction.setText("Show UI");
      showUiAction.setIcon(QIcon("app_show.svg"));

      QMetaObject::Connection connection =
        QObject::connect(&showUiAction, &QAction::triggered, &m_Ui, &Ui::onShow);

      menu->exec(e->globalPos());

      QObject::disconnect(connection);

      QWebView::contextMenuEvent(e);
    }


  private:
    Ui m_Ui;
};

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_HPP
