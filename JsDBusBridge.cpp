#ifndef _QUADRO_JSDBUSBRIDGE_CPP
#define _QUADRO_JSDBUSBRIDGE_CPP

#include "JsDBusBridge.hpp"

const QDBusArgument &
operator<<(const QDBusArgument & arg, const Quadro::Foo & foo)
{
  return arg;
}

const QDBusArgument &
operator>>(const QDBusArgument & arg, Quadro::Foo & foo)
{
  return arg;
}

// Q_DECLARE_METATYPE(QDBusConnection)

#endif // _QUADRO_JSDBUSBRIDGE_CPP
