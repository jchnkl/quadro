#ifndef _QUADRO_WEBVIEW_CPP
#define _QUADRO_WEBVIEW_CPP

#include "WebView.hpp"
#include "NetWmWindowType.hpp"

#include <iostream>
#include <QWebFrame>

namespace Quadro {

void
WebPage::javaScriptConsoleMessage(const QString & message,
                                  int lineNumber,
                                  const QString & sourceID)
{
  std::cerr << "<"
            << sourceID.toStdString()
            << "|"
            << std::to_string(lineNumber)
            << "> "
            << message.toStdString()
            << std::endl;
}

void
WebView::onLoadUrl(const QString & url)
{
  this->load(QUrl::fromUserInput(url));
}

WebView::WebView(const Config & config)
  : m_Ui(this)
{
  this->setPage(&m_WebPage);

  // create & set transparent palette for browser window
  QPalette palette = this->palette();
  palette.setBrush(QPalette::Base, Qt::transparent);
  this->page()->setPalette(palette);

  // set object name for style sheet
  this->setObjectName("QuadroWebView");

  // hint for window manager to leave away frames/window borders
  this->setWindowFlags(Qt::FramelessWindowHint);

  // make window background transparent
  this->setAttribute(Qt::WA_TranslucentBackground, true);

  Ewmh ewmh(QX11Info::connection());
  NetWmWindowType windowType(ewmh, this->winId());

  windowType.clear(NetWmWindowType::Same).add(config.windowTypeHint());

  connect(&m_Ui, &Ui::loadUrl, this, &WebView::onLoadUrl);
  connect(this, &QWebView::urlChanged, &m_Ui, &Ui::onUrlChanged);
  connect(this->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
          this, &WebView::onJsWindowObjectCleared);
  connect(this->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
          &m_DBus, &DBus::reset);

  connect(&m_Ui, &Ui::moveBy, this, &WebView::onMoveBy);
  connect(&m_Ui, &Ui::resizeBy, this, &WebView::onResizeBy);

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

void
WebView::onMoveBy(const QPoint & offset)
{
  this->move(this->pos() + offset);
}

void
WebView::onResizeBy(const QMargins & offset)
{
  this->setGeometry(this->geometry() + offset);
}

void
WebView::resizeEvent(QResizeEvent * e)
{
  m_Ui.resize(e->size());
  QWebView::resizeEvent(e);
}

void
WebView::contextMenuEvent(QContextMenuEvent * e)
{
  if (m_Ui.isVisible()) {
    QMenu menu;

    QAction * hideUiAction = menu.addAction("Hide UI");
    hideUiAction->setIcon(QIcon("app_hide.svg"));

    QMetaObject::Connection connection =
      QObject::connect(hideUiAction, &QAction::triggered, &m_Ui, &Ui::onHide);

    menu.exec(e->globalPos());

    QObject::disconnect(connection);

  } else {
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
  }
}

void
WebView::onJsWindowObjectCleared(void)
{
  this->page()->mainFrame()->addToJavaScriptWindowObject("dbus", &m_DBus);
}

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_CPP
