#include <iostream>
#include <QPixmap>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QWebView>
#include <QX11Info>
#include <xcb/xcb.h>

void
xcbSetNetWmWindowTypeHint(const QWidget & widget, const std::string & hint)
{
  xcb_connection_t * c = QX11Info::connection();

  std::string type = "_NET_WM_WINDOW_TYPE";

  xcb_intern_atom_cookie_t type_atom =
    xcb_intern_atom(c, 0, type.length(), type.c_str());
  xcb_intern_atom_cookie_t hint_atom =
    xcb_intern_atom(c, 0, hint.length(), hint.c_str());

  xcb_intern_atom_reply_t * type_reply =
    xcb_intern_atom_reply(c, type_atom, NULL);
  xcb_intern_atom_reply_t * hint_reply =
    xcb_intern_atom_reply(c, hint_atom, NULL);

  const uint32_t data[] = { hint_reply->atom };

  xcb_change_property(c, XCB_PROP_MODE_REPLACE, widget.winId(),
      type_reply->atom, XCB_ATOM_ATOM,
      (u_int8_t)32, sizeof(data)/sizeof(data[0]), &data);
}

class Browser : public QGroupBox
{
  public:
    Browser(void)
    {
      // create & set transparent palette for browser window
      QPalette palette = m_View.palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      m_View.page()->setPalette(palette);

      // enable transparency for underlying window
      m_View.setAttribute(Qt::WA_TranslucentBackground, true);

      m_Layout.addWidget(&m_UrlBar, 0, 0);
      m_Layout.addWidget(&m_View, 1, 0);

      this->setWindowFlags(Qt::FramelessWindowHint);
      this->setAttribute(Qt::WA_TranslucentBackground, true);
      // this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
      // m_View.setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
      // m_Layout.setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
      // m_UrlBar.setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
      this->setLayout(&m_Layout);

      xcbSetNetWmWindowTypeHint(*this, "_NET_WM_WINDOW_TYPE_DESKTOP");

      this->setObjectName("Browser");
      this->setStyleSheet("QGroupBox#Browser { border:0px }");

      // this->winId();

      // this->setW
      // this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);

      connect(&m_View, &QWebView::loadFinished, this, &Browser::renderToImage);
      connect(&m_UrlBar, &QLineEdit::returnPressed, this, &Browser::loadUrlFromBar);
    }

    void loadUrl(const QUrl & url)
    {
      m_View.load(url);
    }

  protected:
    void loadUrlFromBar(void)
    {
      loadUrl(QUrl(m_UrlBar.text(), QUrl::TolerantMode));
      m_UrlBar.selectAll();
    }

    void
    renderToImage(bool ok)
    {
      if (ok) {
        QPixmap pixmap(m_View.size());
        m_View.render(&pixmap, QPoint(), m_View.rect());
        pixmap.save("foo.jpg");
      }
      std::cerr << "done" << std::endl;
    }

    void
    enterEvent(QEvent *)
    {
      m_UrlBar.show();
    }

    void
    leaveEvent(QEvent *)
    {
      m_UrlBar.hide();
    }

  private:
    QGridLayout m_Layout;
    QLineEdit m_UrlBar;
    QWebView m_View;
}; // Browser

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);

  Browser browser;
  browser.loadUrl(QUrl(argv[1]));
  browser.show();

  return app.exec();
}
