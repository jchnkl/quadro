#ifndef _QUADRO_BROWSER_HPP
#define _QUADRO_BROWSER_HPP

#include <QApplication>
#include <QContextMenuEvent>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QX11Info>

#include "Ui.hpp"
#include "Config.hpp"
#include "WebView.hpp"
#include "NetWmWindowType.hpp"

namespace Browser {

class Window
  : public QGroupBox
{
  public:
    Window(const Config & config)
    {
      // set object name for style sheet
      this->setObjectName("BrowserWindow");

      // remove the border
      this->setStyleSheet("QGroupBox#BrowserWindow { border: 0px; }");

      // hint for window manager to leave away frames/window borders
      this->setWindowFlags(Qt::FramelessWindowHint);

      // make window background transparent
      this->setAttribute(Qt::WA_TranslucentBackground, true);

      Ewmh ewmh(QX11Info::connection());
      NetWmWindowType windowType(ewmh, this->winId());

      windowType.clear(NetWmWindowType::Same).add(config.windowTypeHint());

      // remove layout margins / padding
      m_Layout.setMargin(0);

      // add widgets to grid layout
      m_Layout.addWidget(&m_Ui);
      m_Layout.addWidget(&m_View);

      // use grid layout
      this->setLayout(&m_Layout);

      // create & set transparent palette for browser window
      QPalette palette = m_View.palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      m_View.page()->setPalette(palette);

      // enable transparency for underlying window
      m_View.setAttribute(Qt::WA_TranslucentBackground, true);

      connect(&m_View, &WebView::urlChanged, this, &Window::onUrlChanged);
      connect(&m_View, &WebView::contextMenuSignal, this, &Window::onContextMenuSignal);
      connect(&m_Ui.urlBar(), &QLineEdit::returnPressed, this, &Window::onReturnPressed);

      this->setGeometry(config.x(), config.y(), config.width(), config.height());

      if (config.hasUrl()) {
        this->loadUrl(QUrl::fromUserInput(config.url()));
      }

      if (config.fullscreen()) {
        QRect desktop_rect = QApplication::desktop()->screenGeometry();
        this->setGeometry(desktop_rect);
      }

      this->show();
    }

    void
    loadUrl(const QUrl & url)
    {
      m_View.load(url);
    }

  protected:
    void
    onShowUi(void)
    {
      if (! m_Ui.isVisible()) {
        m_Ui.show();
      } else {
        m_Ui.hide();
      }
    }

    void
    onContextMenuSignal(QContextMenuEvent * e, QWebView * webview)
    {
      QMenu * menu = webview->page()->createStandardContextMenu();

      QAction showUiAction(menu->addSeparator());;
      showUiAction.setText("Show UI");
      showUiAction.setCheckable(true);
      showUiAction.setChecked(m_Ui.isVisible());

      QMetaObject::Connection connection =
        QObject::connect(&showUiAction, &QAction::changed, this, &Window::onShowUi);

      menu->addAction(&showUiAction);
      menu->exec(e->globalPos());

      QObject::disconnect(connection);
    }

    void
    onUrlChanged(const QUrl & url)
    {
      m_Ui.urlBar().setText(url.toString());
    }

    void
    onReturnPressed(void)
    {
      loadUrl(QUrl::fromUserInput(m_Ui.urlBar().text()));
      m_Ui.urlBar().selectAll();
    }

  private:
    Ui m_Ui;
    WebView m_View;
    QVBoxLayout m_Layout;
}; // class Window

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
