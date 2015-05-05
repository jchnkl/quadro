#ifndef _QUADRO_JSDBUSBRIDGE_HPP
#define _QUADRO_JSDBUSBRIDGE_HPP

#include <iostream>
#include <algorithm>
#include <QtDBus>
#include <QObject>
#include <QVariant>

QVariant
toVariant(const QVariant & variant);

QVariant
toVariant(const QDBusArgument & arg);

class DBusConnection
  : public QObject
{
  Q_OBJECT

  public:
    virtual const QDBusConnection & bus(void) const = 0;

    Q_INVOKABLE
    QVariant
    call(const QString & service,
         const QString & path,
         const QString & interface,
         const QString & method,
         const QVariant & arg1 = QVariant(),
         const QVariant & arg2 = QVariant(),
         const QVariant & arg3 = QVariant(),
         const QVariant & arg4 = QVariant(),
         const QVariant & arg5 = QVariant(),
         const QVariant & arg6 = QVariant(),
         const QVariant & arg7 = QVariant(),
         const QVariant & arg8 = QVariant()) const;


}; // class DBusConnection

class DBusSystemConnection
  : public DBusConnection
{
  public:
    DBusSystemConnection(void);
    DBusSystemConnection(const DBusSystemConnection &);
    const QDBusConnection & bus(void) const;

  private:
    QDBusConnection m_SystemBus;
}; // class DBusSystemConnection

class DBusSessionConnection
  : public DBusConnection
{
  public:
    DBusSessionConnection(void);
    DBusSessionConnection(const DBusSessionConnection &);
    const QDBusConnection & bus(void) const;

  private:
    QDBusConnection m_SessionBus;
}; // class DBusSessionConnection

namespace Quadro {

class DBus
  :public QObject
{
  Q_OBJECT
  Q_PROPERTY(DBusConnection * system READ system)

  signals:
    void propertiesChanged(const QVariant &);

  public slots:

    void onSignal(const QDBusMessage & msg);

  public:

    DBusConnection * system(void);

    // {
    // }



    Q_INVOKABLE
      // void
    QVariant
    // call(QDBusConnection * bus,
    call(
         const DBusConnection & c,
         const QString & service,
         const QString & path,
         const QString & interface,
         const QString & method,
         const QVariant & arg1 = QVariant(),
         const QVariant & arg2 = QVariant(),
         const QVariant & arg3 = QVariant(),
         const QVariant & arg4 = QVariant(),
         const QVariant & arg5 = QVariant(),
         const QVariant & arg6 = QVariant(),
         const QVariant & arg7 = QVariant(),
         const QVariant & arg8 = QVariant())
    {
      QDBusMessage msg =
        QDBusInterface(service, path, interface, c.bus()).call(
        // QDBusInterface(service, path, interface, *bus).call(
            method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            // method);

      // qDebug() << "msg: " << msg;
      qDebug() << "msg.type(): " << msg.type();
      if (msg.type() == QDBusMessage::ReplyMessage) {
        QVariantList result;
        // m_Reply.clear();
        QVariantList variants = msg.arguments();

        for (QVariant variant : variants) {
          result.append(toVariant(variant));
        }

        qDebug() << "result: " << result;
        // qDebug() << "result.length(): " << m_Reply.length();

        // return result;

        if (result.length() == 1) {
          return result.at(0);
        } else {
          return result;
        }

      } else {
        return QVariant();
      }
    }

  protected:

  private:
    // QVariantList m_Reply;
    DBusSystemConnection m_SystemConnection;
    DBusSessionConnection m_SessionConnection;

}; // class DBus

/*
class JsDBusBridge
  : public QObject
{
  Q_OBJECT
  // Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
  // Q_PROPERTY(QString currentNetwork READ currentNetwork)
  // Q_PROPERTY(QVariant connectionStatus READ connectionStatus)
  // Q_PROPERTY(QJsonObject jsonObj READ jsonObj)
  // Q_PROPERTY(QJsonDocument jsonDoc READ jsonDoc)
  // Q_PROPERTY(QVariantMap variantMap READ variantMap)
  Q_PROPERTY(QVariantList variantList READ variantList)

  signals:
    // void propertiesChanged(const QVariantMap &);
    void propertiesChanged(const QVariant &);

  public slots:
    void
    // onPropertiesChanged(const QVariantMap & var)
    // onPropertiesChanged(const QVariant & var)
    // onPropertiesChanged(const QDBusVariant & var)
    onPropertiesChanged(const QDBusArgument & var)
    {
      qDebug() << __PRETTY_FUNCTION__;
      // emit propertiesChanged(var);
    }

    void
    onNameOwnerChanged(QString s1, QString s2, QString s3)
    {
      qDebug() << __PRETTY_FUNCTION__ << ": " << s1 << " | " << s2 << " | " << s3;
    }

    void
    onNameOwnerChanged(QVariant)
    {
      qDebug() << __PRETTY_FUNCTION__;
    }

    void
    onNameOwnerChanged(void)
    {
      qDebug() << __PRETTY_FUNCTION__;
    }

  public:
    JsDBusBridge(QObject * parent = NULL)
      : QObject(parent)
      // , m_SystemBus(QDBusConnection::systemBus())
    // {}

    {
      QDBusConnection m_SystemBus(QDBusConnection::systemBus());

//       {
//         auto service_name   = "org.freedesktop.DBus";
//         auto path_name      = "/org/freedesktop/DBus";
//         auto interface_name = "org.freedesktop.DBus.Introspectable";
//         QDBusInterface interface(service_name, path_name, interface_name, m_SystemBus);
//
//         QDBusMessage msg = interface.call("Introspect");
//
//         QList<QVariant> variants = msg.arguments();
//         qDebug() << msg;
//
//         for (QVariant variant : variants) {
//           m_VariantList.append(toVariant(variant));
//         }
//       }

//       {
//         auto service_name   = "org.freedesktop.NetworkManager";
//         auto path_name      = "/org/freedesktop/NetworkManager/ActiveConnection/2";
//         auto interface_name = "org.freedesktop.DBus.Properties";
//         QDBusInterface interface(service_name, path_name, interface_name, m_SystemBus);
//
//         auto arg_1 = "org.freedesktop.NetworkManager.Connection.Active";
//         QDBusMessage msg = interface.call("GetAll", arg_1);
//
//         QList<QVariant> variants = msg.arguments();
//         qDebug() << msg;
//
//         for (QVariant variant : variants) {
//           m_VariantList.append(toVariant(variant));
//         }
//       }

//       {
//         QDBusInterface iface("org.freedesktop.UPower",
//                              "/org/freedesktop/UPower",
//                              "org.freedesktop.DBus.Properties",
//                              m_SystemBus);
//
//         QDBusMessage msg = iface.call("GetAll", "org.freedesktop.UPower");
//         QVariantList variants = msg.arguments();
//         // QVariantList variants = iface.call("GetAll", "org.freedesktop.UPower").arguments();
//
//         qDebug() << msg;
//
//         for (QVariant variant : variants) {
//           m_VariantList.append(toVariant(variant));
//         }
//       }

//       {
//         auto service_name   = "org.freedesktop.NetworkManager";
//         auto path_name      = "/org/freedesktop/NetworkManager/AccessPoint/3";
//         auto interface_name = "org.freedesktop.NetworkManager.AccessPoint";
//         auto signal_name    = "PropertiesChanged";
//
//         qDebug() << "PropertiesChange deliverable: " <<
//           m_SystemBus.connect(
//             service_name, path_name, interface_name, signal_name,
//             // "a{sv}",
//             // "a{si}",
//             // this, SLOT(onPropertiesChanged(QVariant)));
//             // this, SLOT(onPropertiesChanged(QDBusVariant)));
//             this, SLOT(onPropertiesChanged(QDBusArgument)));
//             // this, SLOT(onPropertiesChanged(QVariantMap)));
//
//       }

//       {
//         auto service_name   = "org.freedesktop.DBus";
//         auto path_name      = "/org/freedesktop/DBus";
//         auto interface_name = "org.freedesktop.DBus";
//         auto signal_name    = "NameOwnerChanged";
//
//         qDebug() << "NameOwnerChange deliverable: " <<
//           m_SystemBus.connect(
//             service_name, path_name, interface_name, signal_name,
//             // "sss",
//             this, SLOT(onNameOwnerChanged(void)));
//
//         qDebug() << "NameOwnerChange deliverable: " <<
//           m_SystemBus.connect(
//             service_name, path_name, interface_name, signal_name,
//             // "sss",
//             this, SLOT(onNameOwnerChanged(QVariant)));
//
//         qDebug() << "NameOwnerChange deliverable: " <<
//           m_SystemBus.connect(
//             service_name, path_name, interface_name, signal_name,
//             // "sss",
//             this, SLOT(onNameOwnerChanged(QString, QString, QString)));
//
//       }

    }

    QVariant
    toVariant(const QDBusArgument & arg)
    {
      qDebug() << "arg.currentSignature(): " << arg.currentSignature()
               << "; arg.currentType(): " << arg.currentType();
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
          arg >> tmp;
          return tmp;
        }

        case QDBusArgument::StructureType:
        {
          // QVariant tmp;
          // arg >> tmp;
          // return toVariant(tmp);
          // return arg.asVariant();

          QVariantList tmp_var_list;
          arg.beginStructure();
          while (! arg.atEnd()) {

            {
            qDebug() << "qDebug 1: " << arg.currentType();
            QVariant var = toVariant(arg);
            qDebug() << "qDebug 2: " << var.type();
            tmp_var_list.push_back(var);
            // uint32_t foo;
            // arg >> foo;
            }

            // {
            // qDebug() << "qDebug 3: " << arg.currentType();
            // QVariant var = toVariant(arg);
            // qDebug() << "qDebug 4: " << var.type();
            // tmp_var_list.push_back(var);
            // }

            // break;

            // uint32_t tmp_var;
            // arg >> tmp_var;
            // tmp_var_list.append(tmp_var);
            // QStringList tmp_var_2;
            // arg >> tmp_var_2;
            // tmp_var_list.append(tmp_var_2);
            // break;

            // tmp_var_list.append(arg.asVariant());
            // break;
            // QDBusArgument tmp_arg = tmp_var.value<QDBusArgument>();
            // tmp_var_list.append(toVariant(tmp_arg));
            // QDBusArgument tmp_arg = tmp_var.value<QDBusArgument>();

            // QDBusVariant var = qvariant_cast<QDBusVariant>(arg.asVariant());
            // qDebug() << var.variant().type();
            // QDBusArgument dbarg = arg.asVariant().value<QDBusArgument>();
            // qDebug() << "here: " << dbarg.currentType() << " | " << dbarg.currentSignature();

          }
          arg.endStructure();
          qDebug() << "tmp_var_list: " << tmp_var_list.length() << "; " << tmp_var_list;
          return tmp_var_list;
        }

        case QDBusArgument::MapType:
        {
          QVariantMap tmp;
          arg >> tmp;
          return tmp;
        }

        case QDBusArgument::UnknownType:
        default:
        {
          return QVariant();
        }
      };
    }

    // QList<QVariant>
    // toVariantList(const QVariant & variant)
    QVariant
    toVariant(const QVariant & variant)
    {
      std::cerr << "variant.type(): " << variant.type() << ": ";
      qDebug() << variant.type();
      switch (variant.type()) {
        case QVariant::UserType:
        {
          return toVariant(variant.value<QDBusArgument>());
        }
        // case QVariant::Map:
        // {
        //   return variant.toMap();
        // }
        // case QVariant::List:
        // {
        //   QList<QVariant> tmp;
        //   for (QVariant & v : variant.value<QList<QVariant>>()) {
        //     tmp.append(toVariantList(v));
        //   }
        //   return tmp;
        // }
        // case QVariant::String:
        // {
        //   return { variant };
        //   break;
        // }
        default:
          return variant;
          break;
      };

      return {};
    }

    const QVariantList &
    variantList(void)
    {
      return m_VariantList;
    }

  protected:

  private:
    // QDBusConnection m_SystemBus;
    QVariantList m_VariantList;
};
*/

}; // namespace Quadro

#endif // _QUADRO_JSDBUSBRIDGE_HPP
