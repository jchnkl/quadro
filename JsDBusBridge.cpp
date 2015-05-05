#ifndef _QUADRO_JSDBUSBRIDGE_CPP
#define _QUADRO_JSDBUSBRIDGE_CPP

#include "JsDBusBridge.hpp"

QVariant
toVariant(const QVariant & variant)
{
  qDebug() << __PRETTY_FUNCTION__ << ": " << variant;
  switch (variant.type()) {
    case QVariant::UserType:

      if (variant.canConvert<QDBusVariant>()) {
        qDebug() << "case QVariant::UserType (QDBusVariant)";
        return toVariant(variant.value<QDBusVariant>().variant());
        // toVariant(qvariant_cast<QDBusVariant>(variant).variant());

      } else if (variant.canConvert<QVariantMap>()) {
        qDebug() << "case QVariant::UserType (QVariantMap)";
        QVariantMap tmp = variant.value<QVariantMap>();
        std::transform(tmp.begin(), tmp.end(), tmp.begin(),
            [&](const QVariant & var)
            {
              return toVariant(var);
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
              return toVariant(var);
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
        return toVariant(variant.value<QDBusArgument>());
      }

    case QVariant::Map:
      qDebug() << "case QVariant::Map";
      return variant;
    case QVariant::List:
      qDebug() << "case QVariant::List";
      return variant;
    default:
      return variant;
      break;
  };
//
//       return QVariant();
}

QVariant
toVariant(const QDBusArgument & arg)
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
        QVariant var = toVariant(arg);
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
            return toVariant(var);
          });

      // return QVariant();
      return tmp;
    }

    case QDBusArgument::StructureType:
    {
      QVariantList tmp_var_list;
      arg.beginStructure();
      while (! arg.atEnd()) {
        QVariant var = toVariant(arg);
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
            return toVariant(var);
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

DBusSystemConnection::DBusSystemConnection(void)
  : m_SystemBus(QDBusConnection::systemBus())
{}

DBusSystemConnection::DBusSystemConnection(const DBusSystemConnection & c)
  : QObject()
  , m_SystemBus(c.m_SystemBus)
{}

DBusSystemConnection &
DBusSystemConnection::operator=(const DBusSystemConnection & c)
{
  this->m_SystemBus = c.m_SystemBus;
  return *this;
}

QDBusConnection &
DBusSystemConnection::bus(void)
{
  return m_SystemBus;
}

namespace Quadro {

DBus::DBus(void)
{
  qRegisterMetaType<DBusConnection *>("DBusConnection *");
  qRegisterMetaType<DBusSystemConnection>("DBusSystemConnection");
  qRegisterMetaType<DBusSessionConnection>("DBusSessionConnection");
}

void
DBus::onSignal(const QDBusMessage & msg)
{
  qDebug() << __PRETTY_FUNCTION__ << ": " << msg;

  QVariantList variants;
  for (const QVariant & variant : msg.arguments()) {
    variants.push_back(toVariant(variant));
  }
  emit propertiesChanged(variants);
}

DBusConnection *
DBus::system(void)
{
  return &m_SystemConnection;
}

}; // namespace Quadro

#endif // _QUADRO_JSDBUSBRIDGE_CPP
