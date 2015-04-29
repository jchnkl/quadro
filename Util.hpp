#ifndef _QUADRO_UTIL_HPP
#define _QUADRO_UTIL_HPP

#include <functional>
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
cursorShape(const QPoint & abspos, const QRect & rect);

void
withAllChildren(QObject * obj, std::function<void(QObject *)> f);

}; // namespace Quadro

#endif // _QUADRO_UTIL_HPP
