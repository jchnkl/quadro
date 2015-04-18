#include <iostream>
#include <memory>
#include <algorithm>
#include <QPixmap>
#include <QApplication>
#include <QLineEdit>
#include <QGroupBox>
#include <QGridLayout>
#include <QWebView>
#include <QX11Info>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>

class Ewmh {
  public:
    Ewmh(xcb_connection_t * const c)
    {
      m_ewmh = std::shared_ptr<xcb_ewmh_connection_t>(new xcb_ewmh_connection_t,
          [](xcb_ewmh_connection_t * ec) {
            xcb_ewmh_connection_wipe(ec);
            std::free(ec);
          });

      xcb_intern_atom_cookie_t * cookies = xcb_ewmh_init_atoms(c, m_ewmh.get());

      if (cookies) {
        xcb_generic_error_t * error = NULL;
        xcb_ewmh_init_atoms_replies(m_ewmh.get(), cookies, &error);
        if (error) {
          std::free(error);
          std::free(cookies);
          throw std::runtime_error(__PRETTY_FUNCTION__);
        }

      } else {
        throw std::runtime_error(__PRETTY_FUNCTION__);
      }
    }

    const xcb_ewmh_connection_t &
    operator*(void) const
    {
      return *m_ewmh;
    }

    xcb_ewmh_connection_t *
    operator()(void) const
    {
      return m_ewmh.get();
    }

    xcb_ewmh_connection_t *
    operator->(void) const
    {
      return m_ewmh.get();
    }

  private:
    std::shared_ptr<xcb_ewmh_connection_t> m_ewmh;
}; // class Ewmh

class NetWmWindowType {
  public:
    enum Hint { Desktop, Dock, Toolbar, Menu, Utility, Splash, Dialog,
                DropdownMenu, PopupMenu, Tooltip, Notification, Combo, Dnd,
                Normal };

    NetWmWindowType(const Ewmh & ewmh, xcb_window_t window)
      : m_ewmh(ewmh), m_window(window)
    {}

    void
    clear(bool all_atoms = false) const
    {
      if (all_atoms) {
        xcb_ewmh_set_wm_window_type(m_ewmh(), m_window, 0, NULL);
      } else {
        auto atoms = get();
        auto f = std::bind(&NetWmWindowType::isHint, this, std::placeholders::_1);
        atoms.erase(std::remove_if(atoms.begin(), atoms.end(), f), atoms.end());
        set(atoms);
      }
    }

    std::vector<xcb_atom_t>
    get(void) const
    {
      xcb_get_property_cookie_t cookie =
        xcb_ewmh_get_wm_window_type_unchecked(m_ewmh(), m_window);

      xcb_generic_error_t * error = NULL;
      xcb_ewmh_get_atoms_reply_t reply;
      xcb_ewmh_get_wm_window_type_reply(m_ewmh(), cookie, &reply, &error);

      if (error) {
        std::free(error);
        throw std::runtime_error(__PRETTY_FUNCTION__);
      } else {
        std::vector<xcb_atom_t> atoms(reply.atoms, reply.atoms + reply.atoms_len);
        return atoms;
      }
    }

    void
    add(Hint hint) const
    {
      auto hints = { hint };
      add(hints.begin(), hints.end());
    }

    template<typename Iterator>
    void
    add(Iterator begin, Iterator end) const
    {
      std::vector<xcb_atom_t> hint_atoms(std::distance(begin, end));
      toAtoms(begin, end, hint_atoms.begin());

      auto atoms = get();
      atoms.insert(atoms.end(), hint_atoms.begin(), hint_atoms.end());

      set(atoms);
    }

    void
    set(Hint hint) const
    {
      auto hints = { hint };
      set(hints.begin(), hints.end());
    }

    template<typename Iterator>
    void
    set(Iterator begin, Iterator end) const
    {
      std::vector<xcb_atom_t> atoms(std::distance(begin, end));
      toAtoms(begin, end, atoms.begin());
      set(atoms);
    }

    bool
    isHint(xcb_atom_t atom) const
    {
      return atom == m_ewmh->_NET_WM_WINDOW_TYPE_DESKTOP
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_DOCK
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_TOOLBAR
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_MENU
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_UTILITY
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_SPLASH
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_DIALOG
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_DROPDOWN_MENU
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_POPUP_MENU
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_TOOLTIP
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_COMBO
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_DND
          || atom == m_ewmh->_NET_WM_WINDOW_TYPE_NORMAL
          ;
    }

    xcb_atom_t
    toAtom(Hint hint) const
    {
      switch(hint) {
        case Desktop:      return m_ewmh->_NET_WM_WINDOW_TYPE_DESKTOP;
        case Dock:         return m_ewmh->_NET_WM_WINDOW_TYPE_DOCK;
        case Toolbar:      return m_ewmh->_NET_WM_WINDOW_TYPE_TOOLBAR;
        case Menu:         return m_ewmh->_NET_WM_WINDOW_TYPE_MENU;
        case Utility:      return m_ewmh->_NET_WM_WINDOW_TYPE_UTILITY;
        case Splash:       return m_ewmh->_NET_WM_WINDOW_TYPE_SPLASH;
        case Dialog:       return m_ewmh->_NET_WM_WINDOW_TYPE_DIALOG;
        case DropdownMenu: return m_ewmh->_NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
        case PopupMenu:    return m_ewmh->_NET_WM_WINDOW_TYPE_POPUP_MENU;
        case Tooltip:      return m_ewmh->_NET_WM_WINDOW_TYPE_TOOLTIP;
        case Notification: return m_ewmh->_NET_WM_WINDOW_TYPE_NOTIFICATION;
        case Combo:        return m_ewmh->_NET_WM_WINDOW_TYPE_COMBO;
        case Dnd:          return m_ewmh->_NET_WM_WINDOW_TYPE_DND;
        case Normal:       return m_ewmh->_NET_WM_WINDOW_TYPE_NORMAL;
      };
      // not reached
      throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    template<typename Hint_Iter, typename Atom_Iter>
    void
    toAtoms(Hint_Iter hint_begin, Hint_Iter hint_end, Atom_Iter atom_begin) const
    {
      auto f = std::bind(&NetWmWindowType::toAtom, this, std::placeholders::_1);
      std::transform(hint_begin, hint_end, atom_begin, f);
    }

  protected:
    void
    set(const std::vector<xcb_atom_t> & atoms) const
    {
      xcb_ewmh_set_wm_window_type(m_ewmh(), m_window, atoms.size(),
                                  const_cast<xcb_atom_t *>(atoms.data()));
    }

  private:
    Ewmh m_ewmh;
    xcb_window_t m_window;
}; // class NetWmWindowType

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
      // set object name for style sheet
      this->setObjectName("Browser");

      // remove the border
      this->setStyleSheet("QGroupBox#Browser { border: 0px; }");

      // hint for window manager to leave away frames/window borders
      this->setWindowFlags(Qt::FramelessWindowHint);

      // make window background transparent
      this->setAttribute(Qt::WA_TranslucentBackground, true);

      // this->setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
      xcbSetNetWmWindowTypeHint(*this, "_NET_WM_WINDOW_TYPE_DESKTOP");

      // add widgets to grid layout
      m_Layout.addWidget(&m_UrlBar, 0, 0);
      m_Layout.addWidget(&m_View, 1, 0);

      // use grid layout
      this->setLayout(&m_Layout);

      // create & set transparent palette for browser window
      QPalette palette = m_View.palette();
      palette.setBrush(QPalette::Base, Qt::transparent);
      m_View.page()->setPalette(palette);

      // enable transparency for underlying window
      m_View.setAttribute(Qt::WA_TranslucentBackground, true);

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
