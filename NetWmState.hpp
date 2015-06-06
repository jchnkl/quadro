#ifndef _NET_WM_STATE_HPP
#define _NET_WM_STATE_HPP

#include <xcb/xcb_ewmh.h>

#include "Ewmh.hpp"

class NetWmState
{
  public:
    NetWmState(const Ewmh & ewmh, xcb_window_t window)
      : m_ewmh(ewmh), m_window(window)
    {}

    void
    requestChange(xcb_ewmh_wm_state_action_t action,
                  xcb_atom_t first_property,
                  xcb_atom_t second_property,
                  xcb_ewmh_client_source_type_t source_indication)
    {
      xcb_ewmh_request_change_wm_state(m_ewmh(),
                                       0,
                                       m_window,
                                       action,
                                       first_property,
                                       second_property,
                                       source_indication);
    }

    void
    requestChangeMaximizeVert(xcb_ewmh_wm_state_action_t action,
                              xcb_ewmh_client_source_type_t source_indication)
    {
      requestChange(action,
                    m_ewmh->_NET_WM_STATE_MAXIMIZED_VERT,
                    XCB_NONE, source_indication);
    }

    void
    requestChangeMaximizeHorz(xcb_ewmh_wm_state_action_t action,
                              xcb_ewmh_client_source_type_t source_indication)
    {
      requestChange(action,
                    m_ewmh->_NET_WM_STATE_MAXIMIZED_HORZ,
                    XCB_NONE, source_indication);
    }

  private:
    Ewmh m_ewmh;
    xcb_window_t m_window;
}; // class NetWmWindowType

#endif // _NET_WM_STATE_HPP
