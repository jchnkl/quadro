#ifndef _QUADRO_JSDBUSBRIDGE_HPP
#define _QUADRO_JSDBUSBRIDGE_HPP

// #include <QContextMenuEvent>
// #include <QDesktopWidget>
// #include <QMenu>
// #include <QWebView>
// #include <QX11Info>

// #include "Ui.hpp"
// #include "Config.hpp"

#include <iostream>
#include <algorithm>
#include <QtDBus>
#include <QObject>
#include <QVariant>

// Q_DECLARE_METATYPE(QDBusConnection)

namespace Quadro {

class Foo
  : public QObject
{
  Q_OBJECT

  public:
    Foo(void)
    {
    }

    Foo(const Foo & foo)
      // : QObject(static_cast<const QObject &>(foo))
    {
    }
};

};

const QDBusArgument &
operator<<(const QDBusArgument & arg, const Quadro::Foo & foo);
// {
//   return arg;
// }

const QDBusArgument &
operator>>(const QDBusArgument & arg, Quadro::Foo & foo);
// {
//   return arg;
// }

Q_DECLARE_METATYPE(Quadro::Foo)

namespace Quadro {

// QList<QVariant>
// toList(const QDBusArgument & dba)
// {
//   QList<QVariant> tmp;
//   dba >> tmp;
//   return tmp;
// }

class DBus
  :public QObject
{
  Q_OBJECT
  // Q_PROPERTY(QVariantList reply READ reply)
  Q_PROPERTY(QDBusConnection * systemBus READ systemBus)
  Q_PROPERTY(QDBusConnection * sessionBus READ sessionBus)

  signals:
    // void propertiesChanged(void);
    // void propertiesChanged(const QString &);
    void propertiesChanged(const QVariantMap &);
    // void propertiesChanged(const QVariantList &);
    // void propertiesChanged(const QDBusArgument &);

  public slots:

    // void
    // onSignal(void)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    //   emit propertiesChanged();
    // }

    void
    onSignal(const QList<QVariant> &)
    {
      qDebug() << __PRETTY_FUNCTION__;
      // emit propertiesChanged();
    }

    // void
    // onSignal(const Foo &)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    //   // emit propertiesChanged();
    // }

    // void
    // onSignal(const QString & str)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    //   emit propertiesChanged(str);
    // }

    // void
    // onSignal(const QStringList &)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    // }

    // void
    // onSignal(const QByteArray &)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    // }

    void
    onSignal(const QVariantMap & map)
    {
      qDebug() << __PRETTY_FUNCTION__;
      emit propertiesChanged(map);
    }

    // void
    // onSignal(const QVariantList & lst)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    //   // emit propertiesChanged(lst);
    // }

    // void
    // onSignal(const QDBusArgument & arg)
    // {
    //   qDebug() << __PRETTY_FUNCTION__;
    //   // emit propertiesChanged(arg);
    // }

  public:
    DBus(void)
      : m_SystemBus(QDBusConnection::systemBus())
      , m_SessionBus(QDBusConnection::sessionBus())
    {
      // qRegisterMetaType<QDBusConnection *>("QDBusConnection *");
      qDBusRegisterMetaType<Foo>();
    }

    // const QVariantList &
    // reply(void)
    // {
    //   return m_Reply;
    // }

    QDBusConnection *
    systemBus(void)
    {
      return &m_SystemBus;
    }

    QDBusConnection *
    sessionBus(void)
    {
      return &m_SessionBus;
    }

    Q_INVOKABLE
    void
    systemConnectPropertyChanged(const QString & service,
                                 const QString & path,
                                 const QString & interface,
                                 const QString & name)
                  // QObject * receiver,
                  // const char * slot)
    {
      // m_SystemBus.connect(service, path, interface, name, receiver, slot);

      qDebug() << __PRETTY_FUNCTION__ << "DELIVERABLE: "
               << m_SystemBus.connect(service, path, interface, name,
                                      // "a{sv}",
                                      this,
                                      // SLOT(onSignal(void)));
                                      // SLOT(onSignal(Foo)));
                                      // SLOT(onSignal(QVariantList)));
                                      // SLOT(onSignal(QString)));
                                      // SLOT(onSignal(QVariantMap)));
                                      // SLOT(onSignal(QStringList)));
                                      // SLOT(onSignal(QByteArray)));
                                      // SLOT(onSignal(QDBusArgument)));
                                      SLOT(onSignal(QList<QVariant>)));

            // service_name, path_name, interface_name, signal_name,
            // // "a{sv}",
            // // "a{si}",
            // // this, SLOT(onPropertiesChanged(QVariant)));
            // // this, SLOT(onPropertiesChanged(QDBusVariant)));
            // this, SLOT(onPropertiesChanged(QDBusArgument)));
            // this, SLOT(onPropertiesChanged(QVariantMap)));
    }

    Q_INVOKABLE
      // void
    QVariant
    // call(QDBusConnection * bus,
    systemCall(
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
        QDBusInterface(service, path, interface, m_SystemBus).call(
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
                return this->toVariant(var);
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
                return this->toVariant(var);
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
                  return this->toVariant(var);
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
                  return this->toVariant(var);
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

  private:
    // QVariantList m_Reply;
    QDBusConnection m_SystemBus;
    QDBusConnection m_SessionBus;

}; // class DBus

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

}; // namespace Quadro

#endif // _QUADRO_JSDBUSBRIDGE_HPP
