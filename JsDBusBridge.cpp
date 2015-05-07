#ifndef _QUADRO_JSDBUSBRIDGE_CPP
#define _QUADRO_JSDBUSBRIDGE_CPP

#include "JsDBusBridge.hpp"

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

void
DBusSignal::onSignal(const QDBusMessage & msg)
{
  emit notify(unmarshall(msg));
}

std::shared_ptr<DBusSignal>
DBusSignal::connect(QDBusConnection & c,
                    const QString & service,
                    const QString & path,
                    const QString & interface,
                    const QString & name)
{
  auto obj = std::make_shared<DBusSignal>();
  bool connected = c.connect(service, path, interface, name,
                             obj.get(), SLOT(onSignal(QDBusMessage)));
  if (connected) {
    return obj;
  } else {
    return nullptr;
  }
}

const QDBusConnection &
DBusConnection::bus(void) const
{
  return const_cast<DBusConnection *>(this)->bus();
}

void
DBusConnection::reset(void)
{
  m_Signals.clear();
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

DBusSignal *
DBusConnection::connect(const QString & service,
                        const QString & path,
                        const QString & interface,
                        const QString & name)
{
  auto sigkey = this->key(service, path, interface, name);
  auto signal = m_Signals.find(sigkey);

  if (signal == m_Signals.end()) {
    auto sigobj = DBusSignal::connect(this->bus(), service, path, interface, name);
    m_Signals[sigkey] = sigobj;
    return sigobj.get();
  } else {
    return signal->get();
  }
}

QString
DBusConnection::key(const QString & a,
                    const QString & b,
                    const QString & c,
                    const QString & d)
{
  return a + b + c + d;
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

void
DBus::reset(void)
{
  m_SystemConnection.reset();
  m_SessionConnection.reset();
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

#endif // _QUADRO_JSDBUSBRIDGE_CPP
