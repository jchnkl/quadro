#ifndef _NET_WM_WINDOW_TYPE_HPP
#define _NET_WM_WINDOW_TYPE_HPP

#include <algorithm>
#include <xcb/xcb_ewmh.h>

#include "Ewmh.hpp"

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

#endif // _NET_WM_WINDOW_TYPE_HPP
