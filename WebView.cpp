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
  : m_Config(config)
{
  this->setPage(&m_WebPage);

  connect(this->page()->mainFrame(), &QWebFrame::javaScriptWindowObjectCleared,
          this, &WebView::onJsWindowObjectCleared);

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
WebView::contextMenuEvent(QContextMenuEvent * e)
{
  QMenu * menu            = this->page()->createStandardContextMenu();
  QMenu * windowMenu      = menu->addMenu("Window Type");
  QAction * desktopAction = windowMenu->addAction("Desktop");
  QAction * dockAction    = windowMenu->addAction("Dock");
  QAction * normalAction  = windowMenu->addAction("Normal");

  QObject::connect(desktopAction, &QAction::triggered, this,
      [&](void) { changeNetWmWindowType(NetWmWindowType::Desktop); });

  QObject::connect(dockAction, &QAction::triggered, this,
      [&](void) { changeNetWmWindowType(NetWmWindowType::Dock); });

  QObject::connect(normalAction, &QAction::triggered, this,
      [&](void) { changeNetWmWindowType(NetWmWindowType::Normal); });

  menu->exec(e->globalPos());
}

void
WebView::onJsWindowObjectCleared(void)
{
  this->page()->mainFrame()->addToJavaScriptWindowObject("DBus", &m_DBus);
  this->page()->mainFrame()->addToJavaScriptWindowObject("File", &m_File);
  this->page()->mainFrame()->addToJavaScriptWindowObject("Stat", &m_Stat);
  this->page()->mainFrame()->addToJavaScriptWindowObject("System", &m_System);
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

void
WebView::changeNetWmWindowType(NetWmWindowType::Hint hint)
{
  this->hide();
  Ewmh ewmh(QX11Info::connection());
  NetWmWindowType windowType(ewmh, this->winId());
  windowType.clear(NetWmWindowType::Same).add(hint);
  this->show();
}

}; // namespace Quadro

#endif // _QUADRO_WEBVIEW_CPP
