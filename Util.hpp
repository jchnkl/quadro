#ifndef _QUADRO_UTIL_HPP
#define _QUADRO_UTIL_HPP

#include <iostream>
#include <functional>
#include <QRect>
#include <QPoint>
#include <QObject>

namespace Quadro {

enum Direction
  { None
  , Left
  , Right
  , Top
  , Bottom
  , TopRight
  , TopLeft
  , BottomRight
  , BottomLeft
  };

Direction
direction(const QPoint & abspos, const QRect & rect);

Qt::CursorShape
cursorShape(Direction d);

Qt::CursorShape
cursorShape(const QPoint & abspos, const QRect & rect);

void
withAllChildren(QObject * obj, std::function<void(QObject *)> f);

}; // namespace Quadro

std::ostream &
operator<<(std::ostream & os, Quadro::Direction d);

#endif // _QUADRO_UTIL_HPP
