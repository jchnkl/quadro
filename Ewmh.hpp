#ifndef _EWMH_HPP
#define _EWMH_HPP

#include <memory>
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

#endif // _EWMH_HPP
