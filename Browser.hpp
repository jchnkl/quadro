#ifndef _QUADRO_BROWSER_HPP
#define _QUADRO_BROWSER_HPP

#include <iostream>

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

void
printChildren(QObject * obj)
{
  for (auto child : obj->children()) {
    std::cerr << "child (0x" << std::hex << child << std::dec << "): \"" << qPrintable(child->objectName()) << "\"" << std::endl;
    printChildren(child);
  }
}

// void
// withAllChildren(QObject * obj, std::function<void(QObject *)> f)
// {
//   for (auto child : obj->children()) {
//     f(child);
//     withAllChildren(child, f);
//   }
// }


namespace Browser {

class Window
  : public QGroupBox
{
  public:
    Window(const Config & config)
    // Window(QApplication * app, const Config & config)
      // : m_app(app)
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

      connect(&m_Ui, &Ui::moveButtonChecked, this, &Window::onMoveButtonChecked);
      connect(&m_Ui, &Ui::resizeButtonChecked, this, &Window::onResizeButtonChecked);
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

    // void
    // mouseMoveEvent(QMouseEvent * e)
    // {
    //   std::cerr << __PRETTY_FUNCTION__ << std::endl;
    //   if (m_MouseAction) {
    //     m_MouseAction->mouseMoveEvent(e);
    //   }
    // }

    // void
    // mousePressEvent(QMouseEvent * e)
    // {
    //   std::cerr << __PRETTY_FUNCTION__ << std::endl;
    //   if (m_MouseAction) {
    //     m_MouseAction->mousePressEvent(e);
    //   }
    // }

    // void
    // mouseReleaseEvent(QMouseEvent * e)
    // {
    //   std::cerr << __PRETTY_FUNCTION__ << std::endl;
    //   if (m_MouseAction) {
    //     m_MouseAction->mouseReleaseEvent(e);
    //   }
    // }

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

    void
    onMoveButtonChecked(bool isChecked)
    {
      std::cerr << __PRETTY_FUNCTION__ << ": " << std::boolalpha << isChecked << std::dec << std::endl;
      // QCursor cursor = this->cursor();

      if (! isChecked) {
        m_EventFilter = std::make_shared<MoveEventFilter>(&m_Ui, this);
        // withAllChildren(this, [&](QObject * child) {
        //     child->installEventFilter(m_EventFilter.get());
        // });
      } else {
        // withAllChildren(this, [&](QObject * child) {
        //     child->removeEventFilter(m_EventFilter.get());
        // });
        m_EventFilter.reset();
      }

      /*
      if (! isChecked) {
        // cursor.setShape(Qt::SizeAllCursor);

        // this->grabMouse(Qt::SizeAllCursor);

        QCursor cursor = this->cursor();
        cursor.setShape(Qt::SizeAllCursor);
        QApplication::setOverrideCursor(cursor);

        m_EventFilter = std::make_shared<MoveEventFilter>(this);

        withAllChildren(this, [&](QObject * child) {
            std::cerr << "child (0x" << std::hex << child
                      << std::dec << "): \"" << qPrintable(child->objectName())
                      << "\"" << std::endl;
            if (! m_Ui.isMoveButton(child)) {
              child->installEventFilter(m_EventFilter.get());
            }
        });

      } else {
        withAllChildren(this, [&](QObject * child) {
            child->removeEventFilter(m_EventFilter.get());
        });

        m_EventFilter.reset();

        QApplication::restoreOverrideCursor();
      }
      */

      // this->setCursor(cursor);

//       if (! isChecked) {
//         m_MoveEventFilter = std::make_shared<MoveEventFilter>(this);
//         this->installEventFilter(m_MoveEventFilter.get());
//         m_View.installEventFilter(m_MoveEventFilter.get());
//
//         std::cerr << "this children" << std::endl;
//         printChildren(this);
//
//         std::cerr << "Ui children" << std::endl;
//         printChildren(&m_Ui);
//
//         std::cerr << "View children" << std::endl;
//         printChildren(&m_View);
//
//       } else {
//         this->removeEventFilter(m_MoveEventFilter.get());
//         m_View.removeEventFilter(m_MoveEventFilter.get());
//         m_MoveEventFilter.reset();
//       }

      // this->setMouseTracking(true);
      // m_View.setMouseTracking(true);
      // if (! isChecked) {
      //   // m_MouseAction = std::make_shared<MouseMovable>(this);
      // } else {
      //   m_MouseAction.reset();
      // }
    }

    void
    onResizeButtonChecked(bool isChecked)
    {
      std::cerr << __PRETTY_FUNCTION__ << ": " << std::boolalpha << isChecked << std::endl;
      // if (! isChecked) {
      //   m_MouseAction = std::make_shared<MouseResizable>(this);
      // } else {
      //   m_MouseAction.reset();
      // }
    }

  private:
    Ui m_Ui;
    WebView m_View;
    QVBoxLayout m_Layout;
    // std::shared_ptr<MouseAction> m_MouseAction;
    // std::shared_ptr<MoveEventFilter> m_MoveEventFilter;
    std::shared_ptr<QObject> m_EventFilter;
}; // class Window

}; // namespace Browser

#endif // _QUADRO_BROWSER_HPP
