#ifndef _QUADRO_WEBVIEW_CPP
#define _QUADRO_WEBVIEW_CPP

#include <QScreen>

#include "WebView.hpp"
#include "NetWmState.hpp"
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
  , m_Config(config)
{
  this->setPage(&m_WebPage);

  connect(&m_Ui, &Ui::loadUrl, this, &WebView::onLoadUrl);
  connect(this, &QWebView::urlChanged, &m_Ui, &Ui::onUrlChanged);
  connect(this->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
          this, &WebView::onJsWindowObjectCleared);

  connect(&m_Ui, &Ui::moveBy, this, &WebView::onMoveBy);
  connect(&m_Ui, &Ui::resizeBy, this, &WebView::onResizeBy);

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

  QScreen * screen = getScreen();
  QRect desktop_rect = screen->geometry();

  int x = config.x().value();
  int y = config.y().value();

  if (config.x().isNegative()) {
    x = desktop_rect.width() - config.width() + x;
  }

  if (config.y().isNegative()) {
    y = desktop_rect.height() - config.height() + y;
  }

  this->setGeometry(x + desktop_rect.x(),
                    y + desktop_rect.y(),
                    config.width(),
                    config.height());

  Ewmh ewmh(QX11Info::connection());
  NetWmWindowType windowType(ewmh, this->winId());
  windowType.clear(NetWmWindowType::Same).add(config.windowTypeHint());

  // this is tricky: must come after window type hint, but before state hint
  this->show();

  if (config.windowTypeHint() == NetWmWindowType::Normal) {
    NetWmState wmState(ewmh, this->winId());

    if (config.fullscreen()) {
      wmState.requestChange(XCB_EWMH_WM_STATE_ADD,
                            ewmh->_NET_WM_STATE_MAXIMIZED_VERT,
                            ewmh->_NET_WM_STATE_MAXIMIZED_HORZ,
                            XCB_EWMH_CLIENT_SOURCE_TYPE_OTHER);

    } else if (config.max_vertical()) {
      wmState.requestChangeMaximizeVert(XCB_EWMH_WM_STATE_ADD,
                                        XCB_EWMH_CLIENT_SOURCE_TYPE_OTHER);

    } else if (config.max_horizontal()) {
      wmState.requestChangeMaximizeHorz(XCB_EWMH_WM_STATE_ADD,
                                        XCB_EWMH_CLIENT_SOURCE_TYPE_OTHER);
    }

  } else {
    if (config.fullscreen()) {
      this->setGeometry(desktop_rect);

    } else if (config.max_vertical()) {
      this->resize(this->width(), desktop_rect.height());

    } else if (config.max_horizontal()) {
      this->resize(desktop_rect.width(), this->height());
    }
  }

  if (config.hasUrl()) {
    this->load(QUrl::fromUserInput(config.url()));
  }
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
  this->page()->mainFrame()->addToJavaScriptWindowObject("DBus", &m_DBus);
  this->page()->mainFrame()->addToJavaScriptWindowObject("File", &m_File);
}

QScreen *
WebView::getScreen(void)
{
  for (auto * screen : QGuiApplication::screens()) {
    if (screen->name() == m_Config.screen()) {
      return screen;
    }
  }
  return QGuiApplication::primaryScreen();
}

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_CPP
