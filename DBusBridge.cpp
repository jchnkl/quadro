#ifndef _QUADRO_DBUSBRIDGE_CPP
#define _QUADRO_DBUSBRIDGE_CPP

#include "DBusBridge.hpp"

QVariant
fromVariant(const QVariant & variant);

QVariant
fromArgument(const QDBusArgument & arg);

QVariant
unmarshall(const QDBusMessage & msg);

QString
toString(const QVariant & variant);

QVariantList &
transformVariants(QVariantList & variants);

QString
toString(const QVariant & variant)
{
  QString result = variant.value<QString>();

  if (! result.isNull()) {
    return result;
  }

  throw std::runtime_error(std::string(__PRETTY_FUNCTION__)
                           + ": cannot convert "
                           + std::string(variant.typeName())
                           + " to QString."
                           + " This is probably a bug in the remote service");
}

template<typename Iterator>
void
transformVariants(Iterator begin, Iterator end)
{
  std::transform(begin, end, begin,
      [&](const QVariant & var)
      {
        return fromVariant(var);
      });
}

QVariant
fromVariant(const QVariant & variant)
{
  switch (variant.type()) {
    case QVariant::UserType:

      if (variant.canConvert<QDBusVariant>()) {
        return fromVariant(variant.value<QDBusVariant>().variant());

      } else if (variant.canConvert<QVariantMap>()) {
        QVariantMap tmp = variant.value<QVariantMap>();
        transformVariants(tmp.begin(), tmp.end());
        return tmp;

      } else if (variant.canConvert<QVariantList>()) {
        QVariantList tmp = variant.value<QVariantList>();
        transformVariants(tmp.begin(), tmp.end());
        return tmp;

      } else if (variant.canConvert<QByteArray>()) {
        return QString(variant.value<QByteArray>().constData());

      } else if (variant.canConvert<QDBusSignature>()) {
        return variant.value<QDBusSignature>().signature();

      } else if (variant.canConvert<QDBusObjectPath>()) {
        return variant.value<QDBusObjectPath>().path();

      } else {
        return fromArgument(variant.value<QDBusArgument>());
      }

    default:
      return variant;
      break;
  };
}

QVariant
fromArgument(const QDBusArgument & arg)
{
  switch (arg.currentType()) {
    case QDBusArgument::BasicType:
    {
      return arg.asVariant();
    }

    case QDBusArgument::VariantType:
    {
      QDBusVariant tmp;
      arg >> tmp;
      return tmp.variant();
    }

    case QDBusArgument::ArrayType:
    {
      QVariantList tmp;

      arg.beginArray();
      while (! arg.atEnd()) {
        QVariant var = fromArgument(arg);
        tmp.push_back(var);
      }
      arg.endArray();

      transformVariants(tmp.begin(), tmp.end());

      return tmp;
    }

    case QDBusArgument::StructureType:
    {
      QVariantList tmp_var_list;
      arg.beginStructure();
      while (! arg.atEnd()) {
        QVariant var = fromArgument(arg);
        tmp_var_list.push_back(var);
      }
      arg.endStructure();
      return tmp_var_list;
    }

    case QDBusArgument::MapType:
    {
      QVariantMap tmp;

      arg.beginMap();
      while (! arg.atEnd()) {
        QVariant key;
        QVariant value;

        arg.beginMapEntry();
        key = fromArgument(arg);
        value = fromArgument(arg);
        arg.endMapEntry();

        // try to convert the key (which can also be an QDBusObjectPath) to
        // a string. On failure, emit a warning and dismiss the entry.
        try {
          QString mapkey = toString(fromVariant(key));
          tmp.insert(mapkey, fromVariant(value));
        } catch (const std::exception & e) {
          qWarning() << e.what();
        }
      }
      arg.endMap();

      return tmp;
    }

    case QDBusArgument::UnknownType:
    default:
    {
      return QVariant();
    }
  };
}

QVariant
unmarshall(const QDBusMessage & msg)
{
  QVariantList variants = msg.arguments();

  transformVariants(variants.begin(), variants.end());

  if (variants.length() == 1) {
    return variants.at(0);
  } else {
    return variants;
  }
}

const QDBusConnection &
DBusConnection::bus(void) const
{
  return const_cast<DBusConnection *>(this)->bus();
}

QVariant
DBusConnection::call(const QString & service,
                     const QString & path,
                     const QString & interface,
                     const QString & method,
                     const QVariant & arg1,
                     const QVariant & arg2,
                     const QVariant & arg3,
                     const QVariant & arg4,
                     const QVariant & arg5,
                     const QVariant & arg6,
                     const QVariant & arg7,
                     const QVariant & arg8) const
{
  QDBusMessage msg =
    QDBusInterface(service, path, interface, this->bus()).call(
        method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);

  if (msg.type() == QDBusMessage::ReplyMessage) {
    return unmarshall(msg);
  } else {
    return QVariant();
  }
}

bool
DBusConnection::attach(const QString & service,
                       const QString & path,
                       const QString & interface,
                       const QString & name)
{
  return this->bus().connect(
      service, path, interface, name, this, SLOT(onSignal(QDBusMessage)));
}

bool
DBusConnection::detach(const QString & service,
                       const QString & path,
                       const QString & interface,
                       const QString & name)
{
  return this->bus().disconnect(
      service, path, interface, name, this, SLOT(onSignal(QDBusMessage)));
}

void
DBusConnection::onSignal(const QDBusMessage & msg)
{
  emit notify({ { "service",   msg.service() }
              , { "path",      msg.path() }
              , { "interface", msg.interface() }
              , { "signal",    msg.member() }
              , { "contents",  unmarshall(msg) }
              });
}

DBusSystemConnection::DBusSystemConnection(void)
  : m_SystemBus(QDBusConnection::systemBus())
{}

DBusSystemConnection::DBusSystemConnection(const DBusSystemConnection & c)
  : m_SystemBus(c.m_SystemBus)
{}

QDBusConnection &
DBusSystemConnection::bus(void)
{
  return m_SystemBus;
}

DBusSessionConnection::DBusSessionConnection(void)
  : m_SessionBus(QDBusConnection::sessionBus())
{}

DBusSessionConnection::DBusSessionConnection(const DBusSessionConnection & c)
  : m_SessionBus(c.m_SessionBus)
{}

QDBusConnection &
DBusSessionConnection::bus(void)
{
  return m_SessionBus;
}

DBus::DBus(void)
{
  qRegisterMetaType<DBusConnection *>();
}

DBusConnection *
DBus::system(void)
{
  return &m_SystemConnection;
}

DBusConnection *
DBus::session(void)
{
  return &m_SessionConnection;
}

#endif // _QUADRO_DBUSBRIDGE_CPP
