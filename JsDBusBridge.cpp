#ifndef _QUADRO_JSDBUSBRIDGE_CPP
#define _QUADRO_JSDBUSBRIDGE_CPP

#include "JsDBusBridge.hpp"

// TODO: Unmarshalling still needs fixing, e.g. for:
// dbus-send --session --print-reply --type=method_call
//   --dest=org.gnome.Identity /org/gnome/Identity
//   org.freedesktop.DBus.ObjectManager.GetManagedObjects

QVariant
fromVariant(const QVariant & variant);

QVariant
fromArgument(const QDBusArgument & arg);

QVariant
unmarshall(const QDBusMessage & msg);

QString
toString(const QVariant & variant);

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

QVariant
fromVariant(const QVariant & variant)
{
  qDebug() << __PRETTY_FUNCTION__ << ": " << variant;
  switch (variant.type()) {
    case QVariant::UserType:

      if (variant.canConvert<QDBusVariant>()) {
        qDebug() << "case QVariant::UserType (QDBusVariant)";
        return fromVariant(variant.value<QDBusVariant>().variant());
        // toVariant(qvariant_cast<QDBusVariant>(variant).variant());

      } else if (variant.canConvert<QVariantMap>()) {
        qDebug() << "case QVariant::UserType (QVariantMap)";
        QVariantMap tmp = variant.value<QVariantMap>();
        std::transform(tmp.begin(), tmp.end(), tmp.begin(),
            [&](const QVariant & var)
            {
              return fromVariant(var);
            });
        return tmp;

      } else if (variant.canConvert<QVariantList>()) {
        qDebug() << "case QVariant::UserType (QVariantList)";
        qDebug() << "QVariantList";
        QVariantList tmp = variant.value<QVariantList>();
        std::transform(tmp.begin(), tmp.end(), tmp.begin(),
            [&](const QVariant & var)
            {
              // qDebug() << "QVariantList";
            qDebug() << __PRETTY_FUNCTION__ << ":"
                     << "\n\tvar.type(): " << var.type()
                     << "\n\tvar: " << var
                     << "\n\tvar.value<QDBusObjectPath>().path(): " << var.value<QDBusObjectPath>().path();
              return fromVariant(var);
            });
        return tmp;

      } else if (variant.canConvert<QByteArray>()) {
        qDebug() << "case QVariant::UserType (QByteArray)";
        return QString(variant.value<QByteArray>().constData());

      } else if (variant.canConvert<QDBusSignature>()) {
        qDebug() << "case QVariant::UserType (QDBusSignature)";
        return variant.value<QDBusSignature>().signature();

      } else if (variant.canConvert<QDBusObjectPath>()) {
        qDebug() << "case QVariant::UserType (QDBusObjectPath)";
        return variant.value<QDBusObjectPath>().path();

      } else {
        qDebug() << "case QVariant::UserType (QDBusArgument)";
        return fromArgument(variant.value<QDBusArgument>());
      }

    default:
      return variant;
      break;
  };
//
//       return QVariant();
}

QVariant
fromArgument(const QDBusArgument & arg)
{
  // qDebug() << "arg.currentType(): " << arg.currentType();
  switch (arg.currentType()) {
    case QDBusArgument::BasicType:
    {
      // QVariant var = arg.asVariant();
      // qDebug() << "var: " << var.type() << " \"" << var << "\"";
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
      // QList<QDBusObjectPath> tmp;
      // arg >> tmp;

      arg.beginArray();
      while (! arg.atEnd()) {
        QVariant var = fromArgument(arg);
        tmp.push_back(var);
      }
      arg.endArray();

      // qDebug() << "arrayType: " << tmp;
      qDebug() << __PRETTY_FUNCTION__ << ": " << tmp.length();
      for (auto d : tmp) {
        qDebug() << "d: " << d;
      }
      std::transform(tmp.begin(), tmp.end(), tmp.begin(),
          [&](const QVariant & var)
          {
            // qDebug() << __PRETTY_FUNCTION__ << ":"
            //          << "\n\tvar.type(): " << var.type()
            //          << "\n\tvar: " << var
            //          // << "\n\tvar.value<QDBusObjectPath>().path(): " << var.value<QDBusObjectPath>().path()
            //          << "\n\tvar.value..: " << var.value<QDBusObjectPath>().path();
            //          ;

            // return var.value<QDBusObjectPath>().path();
            return fromVariant(var);
          });

      // return QVariant();
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
      arg >> tmp;

      std::transform(tmp.begin(), tmp.end(), tmp.begin(),
          [&](const QVariant & var)
          {
            // qDebug() << __PRETTY_FUNCTION__ << ":"
            //          << "\n\tvar.type(): " << var.type()
            //          << "\n\tvar: " << var
            //          << "\n\tvar.value<QDBusObjectPath>().path(): " << var.value<QDBusObjectPath>().path();
            return fromVariant(var);
          });

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

  std::transform(variants.begin(), variants.end(), variants.begin(),
      [&](const QVariant & variant)
      {
        return fromVariant(variant);
      });

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
