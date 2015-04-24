#ifndef _QUADRO_UTIL_HPP
#define _QUADRO_UTIL_HPP

#include <functional>
#include <QObject>

void
withAllChildren(QObject * obj, std::function<void(QObject *)> f)
{
  for (auto child : obj->children()) {
    f(child);
    withAllChildren(child, f);
  }
}

#endif // _QUADRO_UTIL_HPP
